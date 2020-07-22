# LGPLv2+ example file

# This is a comment
FROM ubuntu:14.04
MAINTAINER James Turnbull <james@example.com> # comment
ENV REFRESHED_AT 2014-06-01

RUN apt-get -yqq update
RUN apt-get install -yqq software-properties-common python-software-properties
RUN add-apt-repository ppa:chris-lea/redis-server
RUN apt-get -yqq update
RUN apt-get -yqq install redis-server redis-tools
RUN apt-get -yqq update # comment

VOLUME [ "/var/lib/redis", "/var/log/redis/" ]

EXPOSE 6379

CMD []
