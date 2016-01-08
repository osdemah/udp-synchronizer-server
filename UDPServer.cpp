//
// Created by hamed on 1/7/16.
//

#include "UDPServer.h"
#include "utils.h"
#include <thread>

UDPServer::UDPServer(boost::asio::io_service& io_service, unsigned short port,
                     std::shared_ptr<std::queue<Message>> queue,
                     std::shared_ptr<std::mutex> mutex)
        : socket_{io_service, udp::endpoint(udp::v4(), port)}, queue_{queue}, mutex_{mutex}
{
    //std::thread t1{&UDPServer::start_receive, this};
    start_receive();
}

void UDPServer::start_receive()
{
    socket_.async_receive_from(
            boost::asio::buffer(receive_buffer_), remote_endpoint_,
            boost::bind(&UDPServer::handle_receive, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
}

void UDPServer::handle_receive(const boost::system::error_code& error,
                    std::size_t size)
{
    if (!error || error == boost::asio::error::message_size)
    {
        std::string message(receive_buffer_.data(), receive_buffer_.data() + size);
        std::unique_lock<std::mutex>lck{*mutex_};
        queue_->push(Message::toMessage(message));
        lck.unlock();
        std::cout << message << std::endl;
        start_receive();
    }
}