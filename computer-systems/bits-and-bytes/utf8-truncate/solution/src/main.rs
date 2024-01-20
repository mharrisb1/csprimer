use std::io::{Read, Write};

fn main() -> std::io::Result<()> {
    let mut reader = std::io::stdin();
    let mut writer = std::io::stdout();
    let mut read_buf = Vec::new();
    let mut write_buf = Vec::new();
    reader.read_to_end(&mut read_buf)?;
    for line in read_buf.split(|&b| b == 0x0A) {
        if !line.is_empty() {
            let mut max_size = line[0] as usize;
            let utf8_encoded = &line[1..];
            let truncated = truncate_utf8(utf8_encoded, &mut max_size);
            write_buf.extend_from_slice(truncated);
            write_buf.push(0x0A);
        }
    }
    writer.write_all(&write_buf)?;
    Ok(())
}

fn truncate_utf8<'a>(utf8_encoded: &'a [u8], n: &'a mut usize) -> &'a [u8] {
    if *n < utf8_encoded.len() {
        while *n > 0 && utf8_encoded[*n] & 0xC0 == 0x80 {
            *n -= 1;
        }
        &utf8_encoded[..*n]
    } else {
        utf8_encoded
    }
}
