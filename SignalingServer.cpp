#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <functional>
#include <iostream>
#include <set>
#include <string>

typedef websocketpp::server<websocketpp::config::asio> websocket_server;

class SignalingServer {
private:
    websocket_server server;
    std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> connections;

    void on_open(websocketpp::connection_hdl hdl) {
        connections.insert(hdl);
    }

    void on_close(websocketpp::connection_hdl hdl) {
        connections.erase(hdl);
    }

    void on_message(websocketpp::connection_hdl hdl, websocket_server::message_ptr msg) {
        // Echo the received message to all connected clients
        for (auto& conn : connections) {
            server.send(conn, msg->get_payload(), msg->get_opcode());
        }
    }

public:
    SignalingServer() {
        server.init_asio();

        server.set_open_handler(std::bind(&SignalingServer::on_open, this, std::placeholders::_1));
        server.set_close_handler(std::bind(&SignalingServer::on_close, this, std::placeholders::_1));
        server.set_message_handler(std::bind(&SignalingServer::on_message, this, std::placeholders::_1, std::placeholders::_2));
    }

    void run(uint16_t port) {
        server.listen(port);
        server.start_accept();
        server.run();
    }
};

int main() {
    SignalingServer server;
    server.run(9002);  // Run the server on port 9002
    return 0;
}
