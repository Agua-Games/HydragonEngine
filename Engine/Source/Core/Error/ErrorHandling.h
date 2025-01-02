namespace Hydragon {
namespace Error {

class Result {
public:
    bool IsSuccess() const { return m_ErrorCode == 0; }
    const char* GetMessage() const { return m_Message.c_str(); }
    
private:
    int m_ErrorCode = 0;
    std::string m_Message;
};

}} // namespace Hydragon::Error 