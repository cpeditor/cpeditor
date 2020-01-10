#ifndef CFTOOLS_HPP
#define CFTOOLS_HPP

#include <QProcess>
#include <BaseFiles.hpp>
namespace Network
{
class CFTools : private Core::Base::Files
{
  public:
    CFTools(int index);

    void submit(QString url, QString lang);
    void killProcess();

    static bool check();
    ~CFTools();

  private:
    QProcess *cftool = nullptr;
    int index;
};
} // namespace Network
#endif // CFTOOLS_HPP
