use std::io::Read;

fn main() {
    let mut reader = std::io::stdin();
    let mut buffer = Vec::new();
    reader.read_to_end(&mut buffer).unwrap();
    let file = pcap::File::from_bytes(&buffer);
    println!("{}", file.header);
    for packet in file.packets.iter() {
        println!("{}", packet.header);
    }
}

mod pcap {
    use packet::Packet;

    pub struct File<'a> {
        pub header: Header,
        pub packets: Vec<Packet<'a>>,
    }

    impl<'a> File<'a> {
        pub fn from_bytes(bytes: &'a [u8]) -> Self {
            let header: Header = (&bytes[0..24]).into();
            let mut packets = Vec::new();
            let mut offset: usize = 24;
            while offset < bytes.len() {
                let packet = Packet::from_bytes(&bytes[offset..]);
                offset += packet.header.captured as usize + 16;
                packets.push(packet);
            }
            Self { header, packets }
        }
    }

    pub struct Header {
        magic: u32,
        major: u16,
        minor: u16,
        tz_o: u32,
        ts_a: u32,
        snapshot: u32,
        llh: u32,
    }

    impl From<&[u8]> for Header {
        fn from(value: &[u8]) -> Self {
            let value: [u8; 24] = value.try_into().unwrap();
            Self {
                magic: u32::from_le_bytes(value[0..4].try_into().unwrap()),
                major: u16::from_le_bytes(value[4..6].try_into().unwrap()),
                minor: u16::from_le_bytes(value[6..8].try_into().unwrap()),
                tz_o: u32::from_le_bytes(value[8..12].try_into().unwrap()),
                ts_a: u32::from_le_bytes(value[12..16].try_into().unwrap()),
                snapshot: u32::from_le_bytes(value[16..20].try_into().unwrap()),
                llh: u32::from_le_bytes(value[20..24].try_into().unwrap()),
            }
        }
    }

    impl std::fmt::Display for Header {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            write!(
                f,
                "FILE:\t🧙=0x{:x}\tversion={}.{}\ttz_offset={}\tts_accuracy={}\t📸={}\t🔗={}",
                self.magic, self.major, self.minor, self.tz_o, self.ts_a, self.snapshot, self.llh
            )
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
            pub ts_s: u32,
            pub ts_ms: u32,
            pub captured: u32,
            pub total: u32,
        }

        impl From<&[u8]> for Header {
            fn from(value: &[u8]) -> Self {
                let value: [u8; 16] = value.try_into().unwrap();
                Self {
                    ts_s: u32::from_le_bytes(value[0..4].try_into().unwrap()),
                    ts_ms: u32::from_le_bytes(value[4..8].try_into().unwrap()),
                    captured: u32::from_le_bytes(value[8..12].try_into().unwrap()),
                    total: u32::from_le_bytes(value[12..16].try_into().unwrap()),
                }
            }
        }

        impl std::fmt::Display for Header {
            fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
                write!(
                    f,
                    "PACKET:\ttime={}.{}\tcaptured={}B\ttotal={}B",
                    self.ts_s, self.ts_ms, self.captured, self.total
                )
            }
        }
    }
}
