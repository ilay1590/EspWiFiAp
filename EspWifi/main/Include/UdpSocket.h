#ifndef UDP_SOCKET_H

class UdpSocket
{
public:
    UdpSocket() = default;
    UdpSocket(UdpSocket &&) = default;
    UdpSocket(const UdpSocket &) = default;
    UdpSocket &operator=(UdpSocket &&) = default;
    UdpSocket &operator=(const UdpSocket &) = default;
    ~UdpSocket() = default;

private:
    
};

#endif // !UDP_SOCKET_H
