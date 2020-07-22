
#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QCoreApplication>
#include <QSharedData>

#include <functional>

class ExternalFwdDecl;
class LocalFwdDecl;

template<typename T> class QList;

class MyObject : public QObject
{
  Q_OBJECT
public:
  MyObject(QObject* parent = nullptr);

  inline MyObject(const QString& inlineCtor, QObject* parent = nullptr);

  enum LocalEnum {
    Val1 = 1,
    Val2
  };
  Q_DECLARE_FLAGS(LocalEnums, LocalEnum)

  enum {
     AnonVal1,
     AnonVal2
  };

  double unnamedParameters(int, double);

  int addThree(int input) const;
  QList<int> addThree(QList<int> input) const;

  const QString addThree(const QString& input, const QString& prefix = QStringLiteral("Default")) const;

  int findNeedle(QStringList list, QString needle, Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith | Qt::MatchWrap)) const;

  int qtEnumTest(QFlags<Qt::MatchFlag> flags);
  int localEnumTest(QFlags<MyObject::LocalEnum> flags);

  inline int inlineMethod(int arg);

  int functionParam(std::function<int()> fn);
  int groups(unsigned int maxCount = std::numeric_limits<uint>::max()) const;

  void enumNullptr(Qt::WindowFlags f = nullptr);

  void enumBraces(Qt::WindowFlags f = {});
  void stringBraces(QString s = {});
  void stringRefBraces(QString const& s = {});
  void intBraces(int i = {});
  void intRefBraces(int const& i = {});
  void pointerBraces(int* p = {});

  int const_parameters(const int input, QObject* const obj = 0) const;

  int externalFwdDecl(const ExternalFwdDecl& f);
  int externalFwdDeclRef(ExternalFwdDecl& f);

  int localFwdDecl(const LocalFwdDecl& f);

  int localListDecl(const QList<int>& l);

  int localDeclListDecl(const QList<LocalFwdDecl>& l);

  mode_t dummyFunc(QObject* parent) { return 0; }

signals:
  void publicSlotCalled();

Q_SIGNALS:
  void privateSlotCalled();
  void protectedSlotCalled();

public slots:
  void publicSlot1();

public Q_SLOTS:
  void publicSlot2();

protected slots:
  void protectedSlot1();

protected Q_SLOTS:
  void protectedSlot2();

private slots:
  void privateSlot1();

private Q_SLOTS:
  void privateSlot2();
};

inline MyObject::MyObject(const QString& inlineCtor, QObject* parent)
  : MyObject(parent)
{

}

inline int MyObject::inlineMethod(int arg)
{
  return arg;
}

class LocalFwdDecl
{
public:
  LocalFwdDecl(int value);

  int getValue() const;

private:
  int m_value;
};

class NonCopyable
{
public:
  NonCopyable();
  ~NonCopyable();

private:
  int* const mNum;
};

class NonCopyableByMacro
{
public:
  NonCopyableByMacro();

  Q_DECLARE_TR_FUNCTIONS(NonCopyableByMacro)

private:
  Q_DISABLE_COPY(NonCopyableByMacro)
};

Q_DECLARE_METATYPE(NonCopyableByMacro*)

class HasPrivateDefaultCtor
{
public:
private:
  HasPrivateDefaultCtor(int param = 0);
};

class Shared : public QSharedData
{
public:
  Shared(const Shared& other);
};

namespace SomeNS {

class NonCopyableInNS
{
public:
  NonCopyableInNS();
  ~NonCopyableInNS();

private:
  int* const mNum;
};

enum MyFlagType {
    EnumValueOne = 0x01,
    EnumValueTwo = 0x02
};
Q_DECLARE_FLAGS(MyFlags, MyFlagType)

qreal useEnum(MyFlags flags = EnumValueOne);

int customMethod(QList<int> const& nums);

typedef QString(*TagFormatter)(const QStringList &languages,
                               const QString &tagName,
                               const QHash<QString, QString> &attributes,
                               const QString &text,
                               const QStringList &tagPath,
                               SomeNS::MyFlagType format);

}

class TypedefUser
{
public:

  void setTagPattern(const QString &tagName,
                     SomeNS::TagFormatter formatter = NULL,
                     int leadingNewlines = 0);
};

int anotherCustomMethod(QList<int> const& nums);

enum __attribute__((visibility("default"))) EnumWithAttributes {
    Foo,
    Bar = 2
};

#define EXPORT __attribute__((visibility("default")))
#define NO_EXPORT __attribute__((visibility("hidden")))

class EXPORT Visible
{
public:
  EXPORT int visible_fn() { return 1; }
  NO_EXPORT int invisible_fn() { return 1; }
};

class NO_EXPORT Invisible
{
public:
  int someApi() { return 1; }
};
