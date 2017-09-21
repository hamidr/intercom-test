extern crate rand;

use mio::{Events, Ready, Poll, PollOpt, Token};
use mio::net::{TcpListener, TcpStream};
use std::net::{SocketAddr, Ipv4Addr, IpAddr};
use std::io::{Read, Write, ErrorKind};
use std::rc::Rc;
use std::collections::HashMap;
use std::fmt;
use self::rand::Rng;

enum SocketResult {
    Closed(Token),
    Data([u8; 256], usize, Rc<TcpStream>),
}

struct WebSocketServer {
    socket: TcpListener,
    sockets: HashMap<Token, Rc<TcpStream>>,
    token_counter: usize,
    poll: Poll,
}

struct Response {
    addr: Vec<SocketAddr>, // Length = x, each SocketAddr is a random IP
                    // and a random port.
}

impl fmt::Display for Response {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Response(addr: {:?})", self.addr)
    }
}

impl WebSocketServer {
    const SERVER_TOKEN: Token = Token(0);
    const CAPACITY: usize = 1024;

    pub fn new(addr: String) -> WebSocketServer {
        let baddr: SocketAddr = addr.parse().expect("127.0.0.1:9191");
        let server_socket = TcpListener::bind(&baddr).unwrap();

        let poll = Poll::new().unwrap();

        // Register the listener
        poll.register(&server_socket,
                      Self::SERVER_TOKEN,
                      Ready::readable(),
                      PollOpt::edge())
            .unwrap();

        WebSocketServer {
            poll: poll,
            token_counter: 1,
            sockets: HashMap::new(),
            socket: server_socket,
        }
    }

    fn convert_to_string(data: [u8; 256]) -> Option<u32> {
        let mut s = String::new();
        for i in data.iter() {
            let c = *i;
            
            if c == ('\r' as u8) {
                break;
            }
            s.push(c as char);
        }

        match s.parse::<u32>() {
            Ok(n) => Some(n),
            _ => None,
        }
    }

    fn accept<'a>(&'a mut self) {
        loop {
            match self.socket.accept() {
                Ok((socket, _)) => {
                    let token = Token(self.token_counter);
                    self.token_counter += 1;

                    self.poll
                        .register(&socket, token, Ready::readable(), PollOpt::edge())
                        .unwrap();

                    self.sockets.insert(token, Rc::new(socket));
                }

                Err(ref e) if e.kind() == ErrorKind::WouldBlock => {
                    break;
                }

                Err(ref e) => panic!("not handling!!!{:?}", e),
            }
        }
    }

    fn read_from_socket(token: Token, socket_ref: &mut Rc<TcpStream>) -> SocketResult {
        let mut buf = [0; 256];
        let mut stream = &**socket_ref;

        match stream.read(&mut buf) {
            Ok(0) => SocketResult::Closed(token),
            Ok(len) => SocketResult::Data(buf, len, socket_ref.clone()),
            _ => SocketResult::Closed(token),
        }
    }

    fn generate_random_sockets(n: u32) -> Response {
        let mut addrs = Vec::new();
        let mut rng = self::rand::thread_rng();
        for _ in 0..n {
            let port = rng.gen::<u16>();
            let (a, b, c, d) = (rng.gen::<u8>(), rng.gen::<u8>(), rng.gen::<u8>(), rng.gen::<u8>());
            let ip = Ipv4Addr::new(a, b, c ,d);
            addrs.push(SocketAddr::new( IpAddr::V4(ip), port));
        }

        Response {
            addr: addrs
        }
    }

    fn react_to_msg(n: u32, socket_ref: Rc<TcpStream>) {
        let mut stream = &*socket_ref;
        let data = Self::generate_random_sockets(n).to_string();
        stream.write(data.as_bytes()).unwrap();
    }

    fn handle_data<'a>(&'a mut self, res: SocketResult) {
        match res {
            SocketResult::Data(buf, _, socket) => {
                match Self::convert_to_string(buf) {
                    Some(n) => {
                        Self::react_to_msg(n, socket);
                    }
                    _ => ()
                }
            }
            SocketResult::Closed(token) => {
                self.sockets.remove(&token);
            }
        }

    }

    pub fn start(&mut self) {
        let mut events = Events::with_capacity(Self::CAPACITY);

        loop {
            self.poll.poll(&mut events, None).unwrap();

            for event in events.iter() {
                match event.token() {
                    Self::SERVER_TOKEN => self.accept(),

                    token => {
                        let result = match self.sockets.get_mut(&token) {
                            Some(socket_ref) => Self::read_from_socket(token, socket_ref),
                            None => panic!("Must not reach here :)")
                        };

                        self.handle_data(result);
                    }
                }
            }
        }

    }
}

pub fn start(addr: String) {
    let mut server = WebSocketServer::new(addr);

    server.start();
}
