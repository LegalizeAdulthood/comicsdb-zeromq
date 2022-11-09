#include <zmq.hpp>

#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

int main()
{
    using namespace std::chrono_literals;

    zmq::context_t ctx(2);
    zmq::socket_t socket(ctx, zmq::socket_type::rep);
    socket.bind("tcp://*:5555");

    while (true)
    {
        std::cout << "Awaiting request...\n";

        zmq::message_t request;
        socket.recv(request, zmq::recv_flags::none);
        std::cout << "Received Hello\n";

        // do some work
        std::this_thread::sleep_for(1000ms);

        zmq::message_t reply(5);
        std::memcpy(reply.data(), "World", 5);
        socket.send(reply, zmq::send_flags::none);
    }
}
