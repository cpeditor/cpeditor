#ifndef CFTOOLS_HPP
#define CFTOOLS_HPP

#include <QProcess>
namespace Network
{
class CFTools
{
  public:
    CFTools();

    void submit();
    void killProcess();

    static bool check();
    ~CFTools();

  private:
    QProcess *cftool = nullptr;
};
} // namespace Network
#endif // CFTOOLS_HPP
