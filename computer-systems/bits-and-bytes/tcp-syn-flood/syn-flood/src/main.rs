use std::io::Read;

fn main() {
    let mut reader = std::io::stdin();
    let mut buffer = Vec::new();
    reader.read_to_end(&mut buffer).unwrap();
    let file = pcap::File::from_bytes(&buffer);
    let mut initiated: f32 = 0.;
    let mut acknowledged: f32 = 0.;
    for packet in file.packets.iter() {
        let ipv4_packet = ipv4::Packet::from_bytes(&packet.data[4..]);
        let tcp_header = tcp::SegmentHeader::from_bytes(ipv4_packet.data);
        if tcp_header.initiated() {
            initiated += 1.;
        }
        if tcp_header.acknowledged() {
            acknowledged += 1.;
        }
    }
    println!(
        "{} packets parsed with {} connections, {} ({:.2}%) acknowledged",
        file.packets.len(),
        initiated,
        acknowledged,
        acknowledged / initiated * 100.
    )
}

mod pcap {
    use packet::Packet;

    pub struct File<'a> {
        pub packets: Vec<Packet<'a>>,
    }

    impl<'a> File<'a> {
        pub fn from_bytes(bytes: &'a [u8]) -> Self {
            let mut packets = Vec::new();
            let mut offset: usize = 24;
            while offset < bytes.len() {
                let packet = Packet::from_bytes(&bytes[offset..]);
                offset += packet.header.captured as usize + 16;
                packets.push(packet);
            }
            Self { packets }
        }
    }

    mod packet {
        pub struct Packet<'a> {
            pub header: Header,
            pub data: &'a [u8],
        }

        impl<'a> Packet<'a> {
            pub fn from_bytes(bytes: &'a [u8]) -> Self {
                let header: Header = (&bytes[0..16]).into();
                let data = &bytes[16..header.captured as usize + 16];
                Self { header, data }
            }
        }

        pub struct Header {
            pub captured: u32,
        }

        impl From<&[u8]> for Header {
            fn from(value: &[u8]) -> Self {
                Self {
                    captured: u32::from_le_bytes(value[8..12].try_into().unwrap()),
                }
            }
        }
    }
}

mod ipv4 {
    pub struct Packet<'a> {
        pub header: Header,
        pub data: &'a [u8],
    }

    impl<'a> Packet<'a> {
        pub fn from_bytes(bytes: &'a [u8]) -> Self {
            let header: Header = (&bytes[0..20]).into();
            let data = &bytes[20..header.total as usize];
            Self { header, data }
        }
    }

    pub struct Header {
        pub total: u16,
    }

    impl From<&[u8]> for Header {
        fn from(value: &[u8]) -> Self {
            Self {
                total: u16::from_be_bytes(value[2..4].try_into().unwrap()),
            }
        }
    }
}

mod tcp {
    pub struct SegmentHeader {
        pub src: u16,
        pub dest: u16,
        flags: u8,
    }

    impl SegmentHeader {
        pub fn from_bytes(bytes: &[u8]) -> Self {
            Self {
                src: u16::from_be_bytes(bytes[0..2].try_into().unwrap()),
                dest: u16::from_be_bytes(bytes[2..4].try_into().unwrap()),
                flags: u8::from_be_bytes(bytes[13..14].try_into().unwrap()),
            }
        }

        pub fn acknowledged(&self) -> bool {
            (self.flags & 0x10) == 0x10 && self.src == 80
        }

        pub fn initiated(&self) -> bool {
            (self.flags & 0x02) == 0x02 && self.dest == 80
        }
    }
}
