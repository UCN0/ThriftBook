#include "gen-cpp/trade_report_types.h"
#include "gen-cpp/TradeHistory.h"
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace TradeReporting;
using boost::shared_ptr;
using boost::make_shared;

int main() {
    shared_ptr<TTransport> trans;
    trans = make_shared<TSocket>("localhost", 9090);
    trans = make_shared<TFramedTransport>(trans);
    auto proto = make_shared<TCompactProtocol>(trans);
    TradeHistoryClient client(proto);

    try {
        trans->open();
        std::string input;
        std::string symbol("APPL");
        TradeReport trade;
        do {
            client.GetLastSale(trade, symbol);
            std::cout << std::boolalpha 
                      << "[Client] trade(" << symbol << "): " 
                      << trade.seq_num << ">  " 
                      << trade.symbol << " " 
                      << trade.size  << " @ " 
                      << trade.price <<  std::endl;
            std::cout << "enter 'q' to quit, anything else to GetLastSale" << std::endl;
            std::getline(std::cin, input);
        } while (input[0] != 'q');
    } catch(TTransportException te) {
        std::cout << "Client caught a TTransportException: " << te.what() << std::endl;
    } catch(TException e) {
        std::cout << "Client caught a TException: " << e.what() << std::endl;
    } catch(...) {
        std::cout << "Client caught an exception" << std::endl;
    } 
    trans->close();
}