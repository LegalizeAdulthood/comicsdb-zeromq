#include <comicsdb.h>
#include <json.h>

#include <zmq.hpp>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <iostream>
#include <string>

namespace {

namespace ComicsDb = comicsdb::v2;

using RapidJsonString = rapidjson::GenericStringRef<char>;

zmq::message_t loadRequest()
{
    rapidjson::Document doc;
    rapidjson::Value &obj = doc.SetObject();
    obj.AddMember(RapidJsonString{"request"}, RapidJsonString{"load"}, doc.GetAllocator());

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
    std::cout << "Received reply '" << reply.to_string() << "'\n";
}

zmq::message_t createRequest()
{
    ComicsDb::Comic comic;
    comic.issue = 34;
    comic.title = "Marvel Team-Up";
    comic.script = ComicsDb::findPerson("Gerry Conway");
    comic.pencils = ComicsDb::findPerson("Sal Buscema");
    comic.inks = ComicsDb::findPerson("Vince Colletta");
    comic.colors = ComicsDb::findPerson("George Roussos");
    comic.letters = ComicsDb::findPerson("Dave Hunt");
    std::string comicJson = toJson(comic);
    rapidjson::Document comicDoc;
    comicDoc.Parse(comicJson.c_str());

    rapidjson::Document doc;
    rapidjson::Value &obj = doc.SetObject();
    obj.AddMember(RapidJsonString{"request"}, RapidJsonString{"create"}, doc.GetAllocator());
    obj.AddMember(RapidJsonString{"comic"}, comicDoc.GetObject(), doc.GetAllocator());

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return zmq::message_t(buffer.GetString());
}

void createComic(zmq::socket_t &socket)
{
    std::cout << "Sending create request...\n";
    socket.send(createRequest(), zmq::send_flags::dontwait);
    zmq::message_t reply;
    socket.recv(reply, zmq::recv_flags::none);
    std::cout << "Received reply '" << reply.to_string() << "'\n";
}

}

int main()
{
    zmq::context_t ctx(2);
    zmq::socket_t socket(ctx, zmq::socket_type::req);
    std::cout << "Connecting to server...\n";
    socket.connect("tcp://localhost:8080");

    load(socket);
    createComic(socket);

    return 0;
}
