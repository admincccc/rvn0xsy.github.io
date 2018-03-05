//dns_enum

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>
#define T_A 1 //Ipv4 address
#define T_NS 2 //Nameserver
#define T_CNAME 5 // canonical name
#define T_SOA 6 /* start of authority zone */
#define T_PTR 12 /* domain name pointer */
#define T_MX 15 //Mail server

class Mythread {
public:
    Mythread();
    ~Mythread();
    bool setThreads(int thread=0);
    bool runThreads(void (*function)(void * object),void * object);
    bool setDetach();
    bool setJoin();
private:
    int _thread = 0;
    std::vector<std::thread> threads;
};

bool Mythread::setThreads(int thread) {
    if(thread < 100){
        _thread = thread;
    }else{
        _thread = 100;
    }
}


bool Mythread::runThreads(void (*function)(void *) , void * object) {
    for (int i = 0; i < _thread; ++i) {
        threads.push_back(std::thread(function,object));

    }
}

bool Mythread::setDetach(){
    for (int i = 0; i < _thread; ++i) {
        threads[i].detach();
    }
}

bool Mythread::setJoin(){
    for (int i = 0; i < _thread; ++i) {
        threads[i].join();
    }
}

Mythread::Mythread() {
    std::cout << "start threads ... " << _thread << std::endl;
}


Mythread::~Mythread() {
    std::cout << "end threads ... " << _thread << std::endl;
}


struct DNS_HEADER
{
    unsigned short id; // identification number

    unsigned char rd :1; // recursion desired
    unsigned char tc :1; // truncated message
    unsigned char aa :1; // authoritive answer
    unsigned char opcode :4; // purpose of message
    unsigned char qr :1; // query/response flag

    unsigned char rcode :4; // response code
    unsigned char cd :1; // checking disabled
    unsigned char ad :1; // authenticated data
    unsigned char z :1; // its z! reserved
    unsigned char ra :1; // recursion available

    unsigned short q_count; // number of question entries
    unsigned short ans_count; // number of answer entries
    unsigned short auth_count; // number of authority entries
    unsigned short add_count; // number of resource entries
};

//Constant sized fields of query structure
struct QUESTION
{
    unsigned short qtype;
    unsigned short qclass;
};

//Constant sized fields of the resource record structure
#pragma pack(push, 1)
struct R_DATA
{
    unsigned short type;
    unsigned short _class;
    unsigned int ttl;
    unsigned short data_len;
};
#pragma pack(pop)

//Pointers to resource record contents
struct RES_RECORD
{
    unsigned char *name;
    struct R_DATA *resource;
    unsigned char *rdata;
};

//Structure of a Query
typedef struct
{
    unsigned char *name;
    struct QUESTION *ques;
} QUERY;

class dns {

public:
    std::string dnsServer;
    std::vector<std::string> _subdomain = {};
public:
    dns();
    dns(std::string dnsServer);
    ~dns();
    void setHostname(std::string hostname);
    void request(std::string hostname);
    void changeToDnsNameFormat(unsigned char* dns,unsigned char* host);
    int  setDnsServer(std::string  dnsServer);
    static void threadRequest(void * hostname);
    void runThread(int threadNum);
    bool addSubdomain(std::string subdomainName);
private:
    void setDnsHeader(struct DNS_HEADER * dns);
    void setQname(unsigned char * hostname);
    void setQinfo(struct QUESTION *qinfo);
    void requestDnsServer(struct DNS_HEADER *dns);
    u_char* ReadName(unsigned char* reader,unsigned char* buffer,int* count);

private:
    int dnsSock;
    std::string _dnsServer;
    unsigned char _hostname[100];
    unsigned char _buff[65535];
    unsigned char * _qname;
    unsigned char * _reader;
    struct RES_RECORD _answers[20],_auth[20],_addit[20];
    std::map<int,std::string> _aResult;
    sockaddr_in _dest,_a;
    std::mutex _mutex;
    std::string _currentHostname;
};


//
// Created by payloads on 18-2-27.
//



dns::dns(){

}

dns::dns(std::string dnsServer){
    setDnsServer(dnsServer);
}

dns::~dns(){

    auto it = _aResult.begin();
    for (; it != _aResult.end(); ++it) {
        std::cout << "[*]" << this->_hostname << " => " << (*it).second << std::endl;
    }
}

/**
 *
 * @param hostname
 */
void dns::request(std::string hostname) {

    strcpy((char *)this->_hostname,hostname.c_str());

    struct DNS_HEADER * dns = NULL;
    struct QUESTION *qinfo = NULL;

    setDnsHeader(dns);
    setQname((unsigned char *)hostname.c_str());
    setQinfo(qinfo);


    requestDnsServer(dns);
    dns = (struct DNS_HEADER*)_buff;



    _reader = &_buff[sizeof(struct DNS_HEADER) + (strlen((const char*)_qname)+1) + sizeof(struct QUESTION)];

    int stop=0;

    for(int i=0;i<ntohs(dns->ans_count);i++)
    {
        _answers[i].name=ReadName(_reader,_buff,&stop);
        _reader = _reader + stop;
        _answers[i].resource = (struct R_DATA*)(_reader);
        _reader = _reader + sizeof(struct R_DATA);
        if(ntohs(_answers[i].resource->type) == 1)
        {
            _answers[i].rdata = (unsigned char*)malloc(ntohs(_answers[i].resource->data_len));

            for(int j=0 ; j<ntohs(_answers[i].resource->data_len) ; j++)
            {
                _answers[i].rdata[j]=_reader[j];
            }

            _answers[i].rdata[ntohs(_answers[i].resource->data_len)] = '\0';

            _reader = _reader + ntohs(_answers[i].resource->data_len);
        }
        else
        {
            _answers[i].rdata = ReadName(_reader,_buff,&stop);
            _reader = _reader + stop;
        }
    }

    printf("\nAnswer Records : %d \n" , ntohs(dns->ans_count) );

    for(int i=0; i < ntohs(dns->ans_count) ; i++)
    {
        //printf("Name : %s ",answers[i].name);

        if( ntohs(_answers[i].resource->type) == T_A) //IPv4 address
        {

            std::string ipAddress;
            long *p;
            p=(long*)_answers[i].rdata;
            _a.sin_addr.s_addr=(*p); //working without ntohl
            //printf("has IPv4 address : %s",inet_ntoa(a.sin_addr));
            ipAddress=inet_ntoa(_a.sin_addr);
            _aResult.insert(std::pair<int,std::string>(i,ipAddress));
        }

        if(ntohs(_answers[i].resource->type)==5)
        {
            printf("has alias name : %s",_answers[i].rdata);
        }

    }
    close(dnsSock);

    auto it = _aResult.begin();
    for (; it != _aResult.end(); ++it) {
        std::cout << "[*]" << hostname << " => " << (*it).second << std::endl;
    }

    for(int i=0; i < ntohs(dns->ans_count) ; i++){
        if(ntohs(_answers[i].resource->type) == 1) //if its an ipv4 address
        {
            free(_answers[i].rdata);
        }
    }
    _aResult.clear();
    this->_hostname[0]='\0';
}

/**
 *
 * @param dns
 * @param host
 */
void dns::changeToDnsNameFormat(unsigned char* dns,unsigned char* host)
{
    int lock = 0 , i;
    strcat((char*)host,".");

    for(i = 0 ; i < strlen((char*)host) ; i++)
    {
        if(host[i]=='.')
        {
            *dns++ = i-lock;
            for(;lock<i;lock++)
            {
                *dns++=host[lock];
            }
            lock++; //or lock=i+1;
        }
    }
    *dns++='\0';
}

/**
 *
 * @param dns
 */
void dns::setDnsHeader(struct DNS_HEADER *dns) {
    dns = (struct DNS_HEADER * )&_buff;
    dns->id = (unsigned short) htons(getpid());
    dns->qr = 0; //This is a query
    dns->opcode = 0; //This is a standard query
    dns->aa = 0; //Not Authoritative
    dns->tc = 0; //This message is not truncated
    dns->rd = 1; //Recursion Desired
    dns->ra = 0; //Recursion not available! hey we dont have it (lol)
    dns->z = 0;
    dns->ad = 0;
    dns->cd = 0;
    dns->rcode = 0;
    dns->q_count = htons(1); //we have only 1 question
    dns->ans_count = 0;
    dns->auth_count = 0;
    dns->add_count = 0;
}

/**
 *
 * @param hostname
 */
void dns::setQname(unsigned char *hostname) {
    _qname = (unsigned char *)&_buff[sizeof(struct DNS_HEADER)];
    changeToDnsNameFormat(_qname,hostname);
}

/**
 *
 * @param qinfo
 */
void dns::setQinfo(struct QUESTION *qinfo) {
    qinfo = (struct QUESTION *)&_buff[sizeof(struct DNS_HEADER)+strlen((const char*)_qname)+1];
    qinfo->qtype= htons(1);
    qinfo->qclass= htons(1);
}


/**
 *
 * @param reader
 * @param buffer
 * @param count
 * @return
 */
u_char * dns::ReadName(unsigned char* reader,unsigned char* buffer,int* count)
{
    unsigned char *name;
    unsigned int p=0,jumped=0,offset;
    int i , j;

    *count = 1;
    name = (unsigned char*)malloc(256);

    name[0]='\0';

    //read the names in 3www6google3com format
    while(*reader!=0)
    {
        if(*reader>=192)
        {
            offset = (*reader)*256 + *(reader+1) - 49152; //49152 = 11000000 00000000 ;)
            reader = buffer + offset - 1;
            jumped = 1; //we have jumped to another location so counting wont go up!
        }
        else
        {
            name[p++]=*reader;
        }

        reader = reader+1;

        if(jumped==0)
        {
            *count = *count + 1; //if we havent jumped to another location then we can count up
        }
    }

    name[p]='\0'; //string complete
    if(jumped==1)
    {
        *count = *count + 1; //number of steps we actually moved forward in the packet
    }

    //now convert 3www6google3com0 to www.google.com
    for(i=0;i<(int)strlen((const char*)name);i++)
    {
        p=name[i];
        for(j=0;j<(int)p;j++)
        {
            name[i]=name[i+1];
            i=i+1;
        }
        name[i]='.';
    }
    name[i-1]='\0'; //remove the last dot
    return name;
}

/**
 * set name server
 * @param dnsServer
 * @return
 */
int dns::setDnsServer(std::string  dnsServer) {
    _dnsServer = dnsServer;
}


void dns::requestDnsServer(struct DNS_HEADER * dns) {
    int i = sizeof(_dest);
    dnsSock = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP); //UDP packet for DNS queries
    _dest.sin_family = AF_INET;
    _dest.sin_port = htons(53);
    _dest.sin_addr.s_addr = inet_addr(_dnsServer.c_str()); //dns servers
    if(sendto(dnsSock,(char*)_buff,sizeof(struct DNS_HEADER) + (strlen((const char*)_qname)+1) + sizeof(struct QUESTION),0,(struct sockaddr*)&_dest,sizeof(_dest)) < 0)
    {
        return;
    }
    if(recvfrom (dnsSock,(char*)_buff , 65536 , 0 , (struct sockaddr*)&_dest , (socklen_t*)&i ) < 0)
    {
        perror("recvfrom failed");
    }

}


void dns::threadRequest(void * DnsRequest) {

    dns * Dns = (dns *)DnsRequest;
    //Dns->mutex.lock();
    static int threadLock = 0;
    while(threadLock< Dns->_subdomain.size()){
        Dns->_mutex.lock();
        struct DNS_HEADER * dns = NULL;
        struct QUESTION *qinfo = NULL;
        Dns->_currentHostname.append(Dns->_subdomain[threadLock]);
        threadLock++;
        char hostBuff[200];
        sprintf(hostBuff,"%s.%s",Dns->_currentHostname.c_str(),Dns->_hostname);
        //std::cout << "Domain => "  << hostBuff << std::endl;
        Dns->setDnsHeader(dns);
        Dns->setQname((unsigned char *)hostBuff);
        memset(hostBuff,0,0);
        Dns->setQinfo(qinfo);
        Dns->_currentHostname.clear();
        Dns->requestDnsServer(dns);
        dns = (struct DNS_HEADER*)Dns->_buff;
        Dns->_reader = &Dns->_buff[sizeof(struct DNS_HEADER) + (strlen((const char*)Dns->_qname)+1) + sizeof(struct QUESTION)];
        int stop=0;

        for(int i=0;i<ntohs(dns->ans_count);i++)
        {
            Dns->_answers[i].name=Dns->ReadName(Dns->_reader,Dns->_buff,&stop);
            Dns->_reader = Dns->_reader + stop;
            Dns->_answers[i].resource = (struct R_DATA*)(Dns->_reader);
            Dns->_reader = Dns->_reader + sizeof(struct R_DATA);
            if(ntohs(Dns->_answers[i].resource->type) == 1) //if its an ipv4 address
            {
                Dns->_answers[i].rdata = (unsigned char*)malloc(ntohs(Dns->_answers[i].resource->data_len));

                for(int j=0 ; j<ntohs(Dns->_answers[i].resource->data_len) ; j++)
                {
                    Dns->_answers[i].rdata[j]=Dns->_reader[j];
                }

                Dns->_answers[i].rdata[ntohs(Dns->_answers[i].resource->data_len)] = '\0';

                Dns->_reader = Dns->_reader + ntohs(Dns->_answers[i].resource->data_len);
            }
            else
            {
                Dns->_answers[i].rdata = Dns->ReadName(Dns->_reader,Dns->_buff,&stop);
                Dns->_reader = Dns->_reader + stop;
            }
        }

        //printf("\nAnswer Records : %d \n" , ntohs(dns->ans_count) );

        for(int i=0; i < ntohs(dns->ans_count) ; i++)
        {
            //printf("Name : %s ",answers[i].name);

            if( ntohs(Dns->_answers[i].resource->type) == T_A) //IPv4 address
            {

                std::string ipAddress;
                long *p;
                p=(long*)Dns->_answers[i].rdata;
                Dns->_a.sin_addr.s_addr=(*p); //working without ntohl
                //printf(" %s has IPv4 address : %s",hostBuff,inet_ntoa(Dns->a.sin_addr));
                ipAddress=inet_ntoa(Dns->_a.sin_addr);
                Dns->_aResult.insert(std::pair<int,std::string>(i,ipAddress));
            }

            if(ntohs(Dns->_answers[i].resource->type)==5)
            {
                //Canonical name for an alias
                //printf("has alias name : %s",Dns->answers[i].rdata);
            }

        }
        close(Dns->dnsSock);


        auto it = Dns->_aResult.begin();
        for (; it != Dns->_aResult.end(); ++it) {
            std::cout << "[*]" << hostBuff << " => " << (*it).second << std::endl;
        }

        for(int i=0; i < ntohs(dns->ans_count) ; i++){
            if(ntohs(Dns->_answers[i].resource->type) == 1) //if its an ipv4 address
            {
                free(Dns->_answers[i].rdata);
            }
        }
        Dns->_aResult.clear();
        Dns->_mutex.unlock();
    }
}


void dns::setHostname(std::string hostname) {
    strcpy((char *)this->_hostname,hostname.c_str());
}

void dns::runThread(int threadNum) {
    // void * host = (void *)this;
    Mythread th;
    th.setThreads(threadNum);

    th.runThreads(this->threadRequest,(void *)this);

    th.setJoin();
}


bool dns::addSubdomain(std::string subdomainName) {
    _subdomain.push_back(subdomainName);
}



int main(int argc,char * argv[]) {
    std::string dictPath("dict.txt");
    if(argc < 2){
        std::cout << "Usage:" << argv[0] << " <domain name> <Threads>" << std::endl;
        std::cout << "This program is developed by the Qingxuan" << std::endl;
        std::cout << "Email: payloads@aliyun.com" << std::endl;
        exit(0);
    }
    std::cout << "string "  << argc<< std::endl;
    if(argc == 4){
        dictPath = argv[3];
    }
    dns  Enum("114.114.114.114");
    std::string hostname = argv[1];
    Enum.setHostname(hostname);
    std::fstream IO(dictPath);
    while(!IO.eof()){
        char lines[100];
        IO.getline(lines,100);
        std::string lin(lines);
        Enum.addSubdomain(lin);
    }
    Enum.runThread(atoi(argv[2]));
}
