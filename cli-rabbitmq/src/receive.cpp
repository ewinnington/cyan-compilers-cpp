#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/deadline_timer.hpp>


#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>


// callback function that is called when the consume operation starts
auto startCb = [](const std::string &consumertag) {

    std::cout << "consume operation started" << std::endl;
};

// callback function that is called when the consume operation failed
auto errorCb = [](const char *message) {

    std::cout << "consume operation failed" << std::endl;
};

// callback operation when a message was received
auto messageCb = [&channel](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered) {

    std::cout << "message received" << std::endl;

    // acknowledge the message
    channel.ack(deliveryTag);
};


/**
 *  Main program
 *  @return int
 */
int main()
{
    // access to the boost asio handler
    // note: we suggest use of 2 threads - normally one is fin (we are simply demonstrating thread safety).
    boost::asio::io_service service(4);

    // handler for libev
    AMQP::LibBoostAsioHandler handler(service);
    
    // make a connection
    AMQP::TcpConnection connection(&handler, AMQP::Address("amqp://guest:guest@localhost:5672/"));
    
    // we need a channel too
    AMQP::TcpChannel channel(&connection);
    
    // start consuming from the queue, and install the callbacks
    channel.consume("my-queue")
        .onReceived(messageCb)
        .onSuccess(startCb)
        .onError(errorCb);

    // run the handler
    // a t the moment, one will need SIGINT to stop.  In time, should add signal handling through boost API.
    return service.run();
}