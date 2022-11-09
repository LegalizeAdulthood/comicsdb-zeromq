#include <comicsdb.h>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <zmq.hpp>

#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

namespace ComicsDb = comicsdb::v2;

namespace {

zmq::message_t load(ComicsDb::ComicDb &db)
{
    db = ComicsDb::load();

    rapidjson::Document doc;
    rapidjson::Value &obj = doc.SetObject();
    rapidjson::Value ids;
    ids.SetArray();
    for (size_t id = 0; id < db.size(); ++id)
    {
        rapidjson::Value value;
        ids.PushBack(id, doc.GetAllocator());
    }
    obj.AddMember(rapidjson::GenericStringRef<char>{"ids"}, ids, doc.GetAllocator());

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    return {buffer.GetString(), buffer.GetSize()};
}

zmq::message_t unknownRequest()
{
    std::string response = "Unknown request";
    return {response.data(), response.size()};
}

zmq::message_t processRequest(zmq::message_t &req, ComicsDb::ComicDb &db)
{
    rapidjson::Document doc;
    doc.Parse(req.to_string().c_str());
    auto getString = [&doc](const char *key) { return doc[key].GetString(); };
    const std::string request = getString("request");
    if (request == "load")
        return load(db);

    return unknownRequest();
}

}

int main()
{
    ComicsDb::ComicDb db;

    zmq::context_t ctx(2);
    zmq::socket_t socket(ctx, zmq::socket_type::rep);
    socket.bind("tcp://*:8080");

    while (true)
    {
        std::cout << "Awaiting request...\n";

        zmq::message_t request;
        socket.recv(request, zmq::recv_flags::none);
        std::cout << "Received request '" << request.to_string() << "'\n";

        // do some work
        socket.send(processRequest(request, db), zmq::send_flags::none);
    }
}
