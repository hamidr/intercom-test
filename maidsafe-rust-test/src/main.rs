extern crate mio;
use std::env;

mod server;
mod client;

fn main() {
    let args: Vec<String> = env::args().collect();

    if let Some(m) = args.get(1) {
        if let Some(s) = args.get(2) {
            if m == "client" {
                println!("client started:");
                client::start(s.clone());
                return;
            } else if m == "server" {
                println!("server started:");
                server::start(s.clone());
                return;
            }
        }
    }

    println!("\
    First run the server: cargo run server 0.0.0.0:9090\n\
    Then run client: cargo run client SERVER_IP:9090\n\
    ");
}
