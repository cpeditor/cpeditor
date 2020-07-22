
#include "external_lib.h"

ExternalFwdDecl::ExternalFwdDecl(int value)
  : m_value(value)
{

}

int ExternalFwdDecl::getValue() const
{
  return m_value;
}
