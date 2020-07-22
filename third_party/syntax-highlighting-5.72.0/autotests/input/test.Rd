% Single-line comment
\name{foo}
#ifdef unix
\alias{bar}
#endif ignored
\title{Foo}
\description{
    We have a list
    \itemize{
        \item foo,
        \item bar.
    }
    Also a table: \tabular{lr}{
        foo \tab \code{foo} \cr
        bar \tab \preformatted{bar}
    }
    \samp error{code}
}
\section{Section}{
    Escaped braces don't need balance \}
    \subsection{Subsection}{
        \subsection{Subsubsection}{
            Sections can be nested.
        }
    }
}
\usage{
foo(a, b = default)
\method{[}{foo}(x, \dots)
}
\newcommand{\macro}{Test#1}
\arguments{
  \item{a}{\if{html}{&pi; =} \Sexpr[stage=build]{4*atan(1)}}
  \item{\dots}{\macro{text} \link[pkg:bar]{fooo}}
}
\value{
  Return a \code{\link{foo}}.
}
\seealso{
  \code{\link{bar}}
}
\examples{
# R comment
% Rdoc comment
foo(1)
\donttest{foo(2)}
\dontrun{foo(3)}
for (i in 1:10)
    foo(i)
}
\keyword{foo}
