use std::net::TcpStream;
use std::io::BufRead;
use std::io::stdin;
use std::io::{Read, Write};

fn convert_str(data: [u8; 2048]) -> String {
    let mut s = String::new();
    for c in data.iter() {
        if *c == ('\r' as u8) {
            break;
        }
        s.push(*c as char);
    }
    s
}

pub fn start(addr: String) {
  let mut stream = TcpStream::connect(addr).unwrap();
  let mut buf = [0; 2048];
  let stdin = stdin();
  for try_line in stdin.lock().lines() {
    let mut line = try_line.unwrap();

    if line == "0" {
      return;
    }

    if let Err(_) = line.parse::<u32>() {
      continue;
    }

    line.push('\r');
    line.push('\n'); // PROTOCOL!!!

    stream.write(line.as_bytes()).unwrap();
    if let Ok(_) = stream.read(&mut buf) {
      let ss = convert_str(buf);
      println!("{}", ss);
    }
  }

}