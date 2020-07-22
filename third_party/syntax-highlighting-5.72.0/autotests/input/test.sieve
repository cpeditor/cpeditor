# Sieve syntax highlighting test
# see RFC 5228

# comments §2.3.

# Single line with alerts TODO ###
/*
 Multiline comment with alerts FIXME
 this also should be folding region
 */

# literal data §2.4.
# numbers §2.4.1.
42
100K
10M
3G

# strings §2.4.2.
"string"
"string\nwith\"special chars"
# not a string
"multi \
 line \
 #with\a \
 continuation"
# not a string
text:
Multiline
string\" "with quotes"
or # comments
this should also be a folding region
.dot
..dot
dotstuffed dot:
..
ending with a single .
.

# encoded characters §2.4.2.4.
# valid
"$${hex:24 24}"
"${unicode:40}"
"$${hex:40}"
"${hex: da }"
"${hex:FE }"
"${HEX: 40}"
"${unicode:40}"
"${UNICODE:40}"
"${UnICoDE:0000040}"
"${Unicode:ff}"
#invalid
"${hex:40"
"${hex:4${hex:30}}"
"${ unicode:40}"
"${Unicode:Cool}"
# semantic errors, syntactically correct
"${hex:400}"
"${unicode:200000}"
"${Unicode:DF01}"

# test lists §2.5.1
if anyof (not exists ["From", "Date"],
          header :contains "from" "fool@example.com") {
    discard;
}
if allof (not exists ["From", "Date"],
          header :contains "from" "fool@example.com") {
    discard;
}

# match types §2.7.1.
:contains
:matches
:is

# comparators §2.7.3.
if header :contains :comparator "i;octet" "Subject" "MAKE MONEY FAST" {
    discard;
}

# comparison against addresses §2.7.4.
:localpart
:domain
:all

# commands §2.9.
keep;
fileinto "inbox.bla";
redirect "test@kde.org";
discard;

# control if §3.1.
# this also should create folding regions
if header :contains "from" "foo" {
    discard;
} elsif header :contains ["subject"] ["$$$"] {
    discard;
} else {
    fileinto "INBOX";
}

# control require §3.2.
require "fileinto";
require ["vacation", "imapflags"];

# control stop §3.3.
stop;

# test commands §5.
address
allof
anyof
exists
false
header
not
size
true

# test address §5.1.
if address :is :all "from" "kde@example.com" {
    discard;
} elsif address :domain :is ["From", "To"] "example.com" { # comment
    keep; # comment
}

# test envelope §5.4.
if envelope :all :is "from" "kde@example.com" {
    discard;
}

# test exists §5.5.
if not exists ["From","Date"] {
    discard;
}

# test header §5.7.
not header :matches "Cc" "?*"

# test size §5.9.
if size :over 500K { discard; }
if size :under 1M { keep; } else { discard; }


/*
 * Extensions
 * see https://www.iana.org/assignments/sieve-extensions/sieve-extensions.xhtml
 */

# Body RFC5173
:raw
:content
:text
if body :raw :contains "MAKE MONEY FAST" {
    discard;
}
if body :content "text" :contains ["bla", "blub"] {
    fileinto "inbox.foo";
}

# Convert RFC6558
require ["convert"];
convert "image/tiff" "image/jpeg" ["pix-x=320","pix-y=240"];

# Copy RFC3894
require ["copy", "fileinto"];
fileinto :copy "incoming";

# Date RFC5260
require ["date", "relational", "fileinto"];
if allof(header :is "from" "boss@example.com",
         date :value "ge" :originalzone "date" "hour" "09",
         date :value "lt" :originalzone "date" "hour" "17")
    { fileinto "urgent"; }
if anyof(date :is "received" "weekday" "0",
         date :is "received" "weekday" "6")
{ fileinto "weekend"; }
if anyof(currentdate :is "weekday" "0",
         currentdate :is "weekday" "6",
         currentdate :value "lt" "hour" "09",
         currentdate :value "ge" "hour" "17")
{ redirect "pager@example.com"; }
if allof(currentdate :value "ge" "date" "2007-06-30",
         currentdate :value "le" "date" "2007-07-07")
{ vacation :days 7  "I'm away during the first week in July."; }

require ["date", "variables", "fileinto"];
if currentdate :matches "month" "*" { set "month" "${1}"; }
if currentdate :matches "year"  "*" { set "year"  "${1}"; }
fileinto "${month}-${year}";

require ["variables", "date", "editheader"];
if currentdate :matches "std11" "*" {addheader "Processing-date" "${0}";}

require ["date", "relational", "index"];
if date :value "gt" :index 2 :zone "-0500" "received" "iso8601" "2007-02-26T09:00:00-05:00"
{ redirect "aftercutoff@example.org"; }

if header :index 1 :matches "received" "*(* [*.*.*.*])*" {}

# Duplicate RFC7352
require ["duplicate", "variables"];
if duplicate { discard; }
if duplicate :header "message-id" { discard; }
if header :matches "message-id" "*" {
    if duplicate :uniqueid "${0}" { discard; }
}

# Editheader RFC5293
addheader "X-Hello" "World";
deleteheader :index 1 "X-Hello";

# Enclose RFC5703
:mime
:anychild
:type
:subtype
:contenttype
:param
replace
enclose
extracttext
if header :mime :type "Content-Type" "image" {}
replace "Executable attachment removed by user filter";
foreverypart {
    if header :mime :param "filename" :matches ["Content-Type", "Content-Disposition"]
        ["*.com", "*.exe", "*.vbs", "*.scr", "*.pif", "*.hta", "*.bat", "*.zip" ] {
        # these attachment types are executable
        enclose :subject "Warning" text:
WARNING! The enclosed message contains executable attachments.
These attachment types may contain a computer virus program
that can infect your computer and potentially damage your data.
.
;
        break;
    }
}
extracttext :first 100 "msgcontent";

# Notify RFC5435
notify :importance "1" :message "This is probably very important" "mailto:kde@example.com";
notify :message "${from_addr}${env_from}: ${subject}" "mailto:kde@example.com";
if not valid_notify_method ["mailto:", "http://gw.example.net/notify?test"] { stop; }
if notify_method_capability "xmpp:tim@example.com?message;subject=SIEVE" "Online" "yes" {}
set :encodeurl "body_param" "stuff";

# Envelope DSN RFC6009
if envelope "notify" "SUCCESS" {}
if allof (envelope "notify" "FAILURE", envelope :comparator "i;ascii-numeric" :count "eq" "notify" "1") {}
if envelope :matches "orcpt" "rfc822;*@example.com" {}
if anyof (envelope :contains "bytimerelative" "-", envelope :value "eq" :comparator "i;ascii-numeric" "bytimerelative" "0") {}
redirect :copy :notify "NEVER" "elsewhere@example.com";
redirect :copy :bytimerelative 600 "cellphone@example.com";
redirect :copy :bytimeabsolute "${date}T20:00:00${zone}" :bymode "return" "cellphone@example.com";

# Environment RFC5183
if environment :contains "item" "" {}

# Reject RFC5429
ereject "I no longer accept mail from this address";
reject text:
Your message is too big.  If you want to send me a big attachment,
put it on a public web site and send me a URL.
.
;

# External Lists RFC6134
if envelope :list "from" ":addrbook:default" {}
if currentdate :list "date" "tag:example.com,2011-01-01:localHolidays" {}
if allof (envelope :detail "to" "mylist", header :list "from" "tag:example.com,2010-05-28:mylist") {
    redirect :list "tag:example.com,2010-05-28:mylist";
}
if string :list "${ip}" "tag:example.com,2011-04-10:DisallowedIPs" {}
if header :mime :param "filename" :list ["Content-Type", "Content-Disposition"] "tag:example.com,2011-04-10:BadFileNameExts" {}

# ihave RFC5463
if ihave "fileinto" {}
error "failed!";

# IMAP Sieve RFC6785
if anyof (environment :is "imap.cause" "APPEND", environment :is "imap.cause" "COPY")  {
    if environment :is "imap.mailbox" "ActionItems" {
        redirect :copy "actionitems@example.com";
    }
}

# IMAP4 Flags RFC5232
setflag "\\Deleted";
setflag "flagvar" "\\Flagged";
addflag "flagvar" "\\Deleted";
addflag "flagvar" ["\\Deleted", "\\Answered"];
removeflag "flagvar" "$MDNRequired";
hasflag :contains "MyVar" "Junk"
hasflag :contains "${MyVar}" ["label", "forward"]
hasflag :count "ge" :comparator "i;ascii-numeric" "MyFlags" 2
fileinto :flags "\\Deleted" "INBOX.bla";

# Include RFC6609
include :personal "always_allow";
include :global "spam_tests";
return;
global "i_am_on_vacation";
set "global.i_am_on_vacation" "1";

# Mailbox RFC5490
if mailboxexists "bla" {}
fileinto :create "inbox.bla";
if metadata :is "INBOX" "/private/vendor/vendor.isode/auto-replies" "on" {}
if metadataexists "INBOX" "/private/vendor/foo" {}
if servermetadata :matches "/private/vendor/vendor.isode/notification-uri" "*" {}
if servermetadataexists "/private/vendor/foo" {}

# Regexp (draft)
if not address :regex ["to", "cc", "bcc"] "me(\\+.*)?@company\\.com" {}

# Relational RFC5231
if address :count "ge" :comparator "i;ascii-numeric" ["to", "cc"] ["3"] {}
if header :value "lt" :comparator "i;ascii-numeric" ["x-priority"] ["3"] {}

# Spamtest RFC5235
if spamtest :value "eq" :comparator "i;ascii-numeric" "0" {}
elsif spamtest :value "ge" :comparator "i;ascii-numeric" "3" {}
elsif spamtest :percent :value "lt" :comparator "i;ascii-numeric" "37" {}
if virustest :value "eq" :comparator "i;ascii-numeric" "4" {}

# Subaddress RFC5233
if envelope :user "to" "postmaster" {}
if envelope :detail "to" "mta-filters" {}

# Vacation RFC5230
vacation "I'm out";
vacation :subject "Automatic response to: ${1}"
                  "I'm away -- send mail to foo in my absence";
vacation :handle "ran-away" "I'm out";
vacation :mime text:
   Content-Type: multipart/alternative; boundary=foo

   --foo

   Hello ${sender}, I'm at the beach relaxing.

   --foo
   Content-Type: text/html; charset=us-ascii

   <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN"
    "http://www.w3.org/TR/REC-html40/strict.dtd">
   <HTML><HEAD><TITLE>How to relax</TITLE>
   <BASE HREF="http://home.example.com/pictures/"></HEAD>
   <BODY><P>I'm at the <A HREF="beach.gif">beach</A> relaxing.
   </BODY></HTML>

   --foo--
.
;
vacation :days 23 :addresses ["kde@example.edu"] "I'm away until October 19.";

# Vacation Seconds RFC6131
vacation :addresses ["kde@example.edu"] :seconds 1800
    "I am in a meeting, and do not have access to email.";

# Variables RFC5229
"&%${}!" # invalid
"${doh!}" #invalid
"${company}"
"bla ${var42} blub"
"${1}"
"${BAD${Company}" # second one is the variable
"${President, ${Company} Inc.}" # inner one is the variable
set "var" "value";
set "var2" "${var}";

set "a" "juMBlEd lETteRS";
set :length "b" "${a}";
set :lower "b" "${a}";
set :upperfirst "b" "${a}";
set :upperfirst :lower "b" "${a}";
set :quotewildcard "b" "Rock*";
set :lowerfirst "b" "${a}";

if string :matches " ${state} " "* pending *" {}

# Dovecot debug extension
require "vnd.dovecot.debug";
if header :contains "subject" "hello" {
    debug_log "Subject header contains hello!";
}
