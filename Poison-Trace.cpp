#include <iostream>
#include <pcap.h>
#include <arpa/inet.h>

void processPacket(u_char* userData, const struct pcap_pkthdr* pkthdr, const u_char* packetData);

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];

    // Capture the network interface for packet sniffing
    pcap_t* handle = pcap_open_live("eth0", BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        std::cerr << "Error opening interface: " << errbuf << std::endl;
        return 1;
    }

    // Start capturing packets
    pcap_loop(handle, 0, processPacket, nullptr);

    // Close the handle
    pcap_close(handle);

    return 0;
}

void processPacket(u_char* userData, const struct pcap_pkthdr* pkthdr, const u_char* packetData) {
    // Parse the ARP packet
    const u_char* arpHeader = packetData + 14; // Skip Ethernet header
    uint16_t arpOpcode = ntohs(*reinterpret_cast<const uint16_t*>(arpHeader + 6));

    // Check if it's an ARP reply packet
    if (arpOpcode == 2) {
        // Print the sender's IP address
        std::cout << "Sender IP: ";
        for (int i = 0; i < 4; ++i) {
            std::cout << static_cast<int>(arpHeader[14 + 8 + 4 + i]);
            if (i != 3) {
                std::cout << ".";
            }
        }
        std::cout << std::endl;

        // Print the sender's MAC address
        std::cout << "Sender MAC: ";
        for (int i = 0; i < 6; ++i) {
            std::cout << std::hex << static_cast<int>(arpHeader[14 + 8 + 10 + i]);
            if (i != 5) {
                std::cout << ":";
            }
        }
        std::cout << std::dec << std::endl;
    }
}
