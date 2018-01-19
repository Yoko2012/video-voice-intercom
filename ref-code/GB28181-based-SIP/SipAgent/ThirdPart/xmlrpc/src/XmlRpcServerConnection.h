#ifndef _XMLRPCSERVERCONNECTION_H_
#define _XMLRPCSERVERCONNECTION_H_
//
// XmlRpc++ Copyright (c) 2002-2003 by Chris Morley
//
#if defined(_MSC_VER)
# pragma warning(disable:4786)    // identifier was truncated in debug info
#endif

#ifndef MAKEDEPEND
# include <string>
#endif

#include "XmlRpcValue.h"
#include "XmlRpcSource.h"

namespace XmlRpc {


  // The server waits for client connections and provides methods
  class XmlRpcServer;
  class XmlRpcServerMethod;

  //! A class to handle XML RPC requests from a particular client
  class XmlRpcServerConnection : public XmlRpcSource {
  public:
    // Static data
    static const char METHODNAME_TAG[];
    static const char PARAMS_TAG[];
    static const char PARAMS_ETAG[];
    static const char PARAM_TAG[];
    static const char PARAM_ETAG[];

    static const std::string SYSTEM_MULTICALL;
    static const std::string METHODNAME;
    static const std::string PARAMS;

    static const std::string FAULTCODE;
    static const std::string FAULTSTRING;

    //! Constructor
    XmlRpcServerConnection(int fd, XmlRpcServer* server, bool deleteOnClose = false);
    //! Destructor
    virtual ~XmlRpcServerConnection();

    // XmlRpcSource interface implementation
    //! Handle IO on the client connection socket.
    //!   @param eventType Type of IO event that occurred. @see XmlRpcDispatch::EventType.
    virtual unsigned handleEvent(unsigned eventType);

    // 用于多线程模式中执行原writeResponse()的任务
    // 如果没有启用多线程,该函数执行原writeResponse()的任务
    bool writeResponseThread();

    void setThreadWorking(bool working){ _threadWorking = working; }
    bool getThreadWorking() { return _threadWorking;}
    
    void setDisconnected() { _isDisconnected = true; }
    bool checkDisconnected() { return _isDisconnected; }

    void virtual close();

  protected:

    bool readHeader();
    bool readRequest();
    bool writeResponse();

    // Parses the request, runs the method, generates the response xml.
    virtual void executeRequest();

    // Parse the methodName and parameters from the request.
    std::string parseRequest(XmlRpcValue& params);

    ///>>>>>BSTAR
    // Added context info to params[0]
    void generateContext(XmlRpcValue& params);

    unsigned handleWrongStatus();

    void setUserCookie(const char* pBuff);

    ///<<<<<BSTAR

    // Execute a named method with the specified params.
    bool executeMethod(const std::string& methodName, XmlRpcValue& params, XmlRpcValue& result);

    // Execute multiple calls and return the results in an array.
    bool executeMulticall(const std::string& methodName, XmlRpcValue& params, XmlRpcValue& result);

    // Construct a response from the result XML.
    void generateResponse(std::string const& resultXml);
    void generateFaultResponse(std::string const& msg, int errorCode = -1);
    std::string generateHeader(std::string const& body);


    // The XmlRpc server that accepted this connection
    XmlRpcServer* _server;

    // Possible IO states for the connection
    enum ServerConnectionState { READ_HEADER, READ_REQUEST, WRITE_RESPONSE };
    ServerConnectionState _connectionState;

    // Request headers
    std::string _header;

    // Referer (request id and sequence number)
    std::string _referer;

    // Number of bytes expected in the request body (parsed from header)
    int _contentLength;

    // Request body
    std::string _request;

    // Response
    std::string _response;

    // Number of bytes of the response written so far
    int _bytesWritten;

    // Whether to keep the current client connection open for further requests
    bool _keepAlive;


    /*
       仅在开启MultiThread后有意义
    */
    // 当前处理的函数是否已在线程中执行结束
    // true表示还在线程执行中
    bool _threadWorking;

    // 表示当前的连接需要被close
    // 发生在连接正在线程处理时,对端断开网络.
    bool _isDisconnected;
    
    
    
    /*
       仅在开启Session Manager后有意义
    */
    // 当前登录的session号,初始为-1
    // 如不为-1,表示已通过验证
    int _sessionId;

    // 当前连接的用户号,由client指定,如客户端不指定,则为0
    int _userSpec;

  };
} // namespace XmlRpc

#endif // _XMLRPCSERVERCONNECTION_H_
