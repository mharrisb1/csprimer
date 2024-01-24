const MAX_SIZE: usize = 7;

fn main() {
    let msg = "hello!!";
    let concealed = conceal(msg);
    let revealed = reveal(concealed);
    assert!(concealed.is_nan());
    assert_eq!(&revealed, msg);
}

fn conceal(s: &str) -> f64 {
    let mut n = s
        .bytes()
        .take(MAX_SIZE)
        .fold(0, |acc, b| (acc << 7) | u64::from(b & 0x7F));
    n |= 0x7FF8000000000000;
    f64::from_be_bytes(n.to_be_bytes())
}

fn reveal(f: f64) -> String {
    let mut n = u64::from_be_bytes(f.to_be_bytes()) & 0x3FFFFFFFFFFFF;
    let mut bytes: Vec<u8> = Vec::with_capacity(MAX_SIZE);
    while n > 0 {
        let part = (n & 0x7F) as u8;
        bytes.push(part);
        n >>= 7;
    }
    bytes.reverse();
    String::from_utf8(bytes).unwrap_or_default()
}
