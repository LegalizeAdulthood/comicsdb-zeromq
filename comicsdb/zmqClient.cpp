#include <zmq.hpp>

#include <iostream>
#include <string>

int main()
{
    zmq::context_t ctx(2);
    zmq::socket_t socket(ctx, zmq::socket_type::req);
    std::cout << "Connecting to server...\n";
    socket.connect("tcp://localhost:5555");

    std::cout << "Sending request...\n";
    socket.send(zmq::str_buffer("Hello, world"), zmq::send_flags::dontwait);

    std::cout << "Awaiting reply....\n";
    zmq::message_t reply;
    socket.recv(reply, zmq::recv_flags::none);
    std::cout << "Received reply '" << reply.to_string() << "'\n";

    return 0;
}
