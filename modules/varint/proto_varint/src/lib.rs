pub fn encode(mut n: u64) -> Vec<u8> {
    let mut out: Vec<u8> = vec![];
    while n > 0 {
        let part = (n & 0x7F) as u8;
        n >>= 7;
        out.push(if n > 0 { part | 0x80 } else { part });
    }
    out
}

pub fn decode(bytes: &[u8]) -> u64 {
    let mut n: u64 = 0;
    for b in bytes.iter().rev() {
        n <<= 7;
        n |= (b & 0x7F) as u64;
    }
    n
}

#[cfg(test)]
mod tests {
    use super::{decode, encode};

    #[test]
    fn test_encode() {
        assert_eq!(encode(1).len(), 1);
        assert_eq!(encode(1), vec![0x1]);
        assert_eq!(encode(150).len(), 2);
        assert_eq!(encode(150), vec![0x96, 0x1]);
        assert_eq!(encode(u64::MAX).len(), 10);
        assert_eq!(
            encode(u64::MAX),
            vec![0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1]
        );
    }

    #[test]
    fn test_decode() {
        assert_eq!(decode(&[0x1]), 1);
        assert_eq!(decode(&[0x96, 0x1]), 150);
        assert_eq!(
            decode(&[0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1]),
            u64::MAX
        );
    }
}
