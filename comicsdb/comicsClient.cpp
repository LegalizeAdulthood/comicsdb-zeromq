#include <zmq.hpp>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <iostream>
#include <string>

namespace {

zmq::message_t loadRequest()
{
    rapidjson::Document doc;
    rapidjson::Value &obj = doc.SetObject();
    obj.AddMember(rapidjson::GenericStringRef<char>{"request"}, rapidjson::GenericStringRef<char>{"load"}, doc.GetAllocator());

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return zmq::message_t(buffer.GetString());
}

void load(zmq::socket_t &socket)
{
    std::cout << "Sending load request...\n";
    socket.send(loadRequest(), zmq::send_flags::dontwait);
    zmq::message_t reply;
    socket.recv(reply, zmq::recv_flags::none);
    std::string replyText(static_cast<const char*>(reply.data()), reply.size());
    std::cout << "Received reply '" << replyText << "'\n";
}

}

int main()
{
    zmq::context_t ctx(2);
    zmq::socket_t socket(ctx, zmq::socket_type::req);
    std::cout << "Connecting to server...\n";
    socket.connect("tcp://localhost:8080");

    load(socket);

    return 0;
}
