# This file is a testcase for the highlighting of Ruby code
# It's not executable code, but a collection of code snippets
#

require 'Config'
require 'DO/Clients'
require 'DO/DBClients'

  def CGI::escapeElement(string, *elements)
    elements = elements[0] if elements[0].kind_of?(Array)
    unless elements.empty?
      string.gsub(/<\/?(?:#{elements.join("|")})(?!\w)(?:.|\n)*?>/ni) do
        CGI::escapeHTML($&)
      end
    else
      string
    end
  end

case inputLine
  when "debug"
    dumpDebugInfo
    dumpSymbols
  when /p\s+(\w+)/
    dumpVariable($1)
  when "quit", "exit"
    exit
  else
    print "Illegal command: #{inputLine}"
end


kind = case year #hi there
         when 1850..1889 then "Blues"
         when 1890..1909 then "Ragtime"
         when 1910..1929 then "New Orleans Jazz"
         when 1930..1939 then "Swing"
         when 1940..1950 then "Bebop"
         else                 "Jazz"
       end

  # URL-encode a string.
  #   url_encoded_string = CGI::escape("'Stop!' said Fred")
  #      # => "%27Stop%21%27+said+Fred"
  def CGI::escape(string)
    string.gsub(/([^ a-zA-Z0-9_.-]+)/n) do
      '%' + $1.unpack('H2' * $1.size).join('%').upcase
    end.tr(' ', '+')
  end


# Class ClientManager
#
# definition : Import, store and export the various data used by the application.
# This class is the sole interface between the application and the underlying database.

mon, day, year = $1, $2, $3 if /(\d\d)-(\d\d)-(\d\d)/
puts "a = #{a}" if fDebug
print total unless total == 0

while gets
  next if /^#/            # Skip comments
  parseLine unless /^$/   # Don't parse empty lines
end

if artist == "John Coltrane" #hi there
  artist = "'Trane" #hi there
end unless nicknames == "no" #hi there

handle = if aSong.artist == "Gillespie" then #hi there
           "Dizzy"
         elsif aSong.artist == "Parker" then
           "Bird"
         else #hi there
           "unknown"
         end

if aSong.artist == "Gillespie" then  handle = "Dizzy"
elsif aSong.artist == "Parker" then  handle = "Bird"
else  handle = "unknown"
end #hi there

if aSong.artist == "Gillespie" then
  handle = "Dizzy"
elsif aSong.artist == "Parker" then
  handle = "Bird"
else
  handle = "unknown"
end

if aSong.artist == "Gillespie"
  handle = "Dizzy"
elsif aSong.artist == "Parker"
  handle = "Bird"
else
  handle = "unknown"
end

 case line
  when /title=(.*)/
    puts "Title is #$1"
  when /track=(.*)/
    puts "Track is #$1"
  when /artist=(.*)/
    puts "Artist is #$1"
end

case shape
  when Square, Rectangle
    # ...
  when Circle
    # ...
  when Triangle
    # ...
  else
    # ...
end 


until playList.duration > 60 #hi there
  playList.add(songList.pop)
end

3.times do
  print "Ho! "
end

loop {
  # block ...
}

songList.each do |aSong|
  aSong.play
end

for aSong in songList
  aSong.play
end

for i in ['fee', 'fi', 'fo', 'fum']
  print i, " "
end
for i in 1..3
  print i, " "
end
for i in File.open("ordinal").find_all { |l| l =~ /d$/}
  print i.chomp, " "
end

class Periods
  def each
    yield "Classical"
    yield "Jazz"
    yield "Rock"
  end
end


periods = Periods.new
for genre in periods
  print genre, " "
end

while gets
  next if /^\s*#/   # skip comments
  break if /^END/   # stop at end
                    # substitute stuff in backticks and try again
  redo if gsub!(/`(.*?)`/) { eval($1) }
  # process line ...
end

i=0
loop do
  i += 1
  next if i < 3
  print i
  break if i > 4
end

for i in 1..100
  print "Now at #{i}. Restart? "
  retry if gets =~ /^y/i
end

def doUntil(cond)
  yield
  retry unless cond
end

i = 0
doUntil(i > 3) {
  print i, " "
  i += 1
}

def system_call
	# ... code which throws SystemCallError
rescue SystemCallError
	$stderr.print "IO failed: " + $!
	opFile.close
	File.delete(opName)
	raise
end

class ClientManager
	
	# constructor
	def initialize(dbase)
		@dbClient = DBClient.new(dbase)
		@clients = Clients.new
	end
	
	def prout(a, b, xy="jj") 24 end 
	###############################################################
	#
	# CLIENTS SECTION
	#
	###############################################################
	
	# update the clients object by retrieving the related data from the database
	# returns the number of clients
	def refreshClients
		@clients.clean
		unless @sqlQuery.nil? then
			@sqlQuery.selectClient.each do |row|
				@clients.addClient(row[0],row[1],row[2],row[3],row[4],row[5], row[6], row[7], row[8])
			end
		else
			puts "SqlQuery wasn't created : cannot read data from database"
		end
		return @clients.length
	end
	
	# insert a client in the database and refreshes the local clients object
	# we assume that none of the arguments is null
	# we assume that the client, identified by raison_sociale doesn't already exists
	def addClient(raison_sociale, division, departement, adresse, cp, ville, nom_contact, tel_contact)
		id = "0"
		unless @sqlQuery.nil? then
			id = @sqlQuery.insertClient(raison_sociale, division, departement, adresse, cp, ville, nom_contact,tel_contact)
		else
			puts "SqlQuery wasn't created : database update cannot be performed"
		end
		@clients.addClient(id, raison_sociale, division, departement, adresse, cp, ville, nom_contact, tel_contact) # synchronize local object with DB
	end
	
	# deletes a client from the database and updates the local Clients object accordingly
	def delClient(nomclient_brut)
		raison_sociale, div, dep = Clients.getIdentifiers(nomclient_brut)
		listeContratsExp, listeContratsSup, listeContratsProd, listePropositionsExp, listePropositionsSup = []

		listeContratsExp = @contratsExpertise.getContratsFromClient(nomclient_brut)
		listeContratsSup = @contratsSupport.getContratsFromClient(nomclient_brut)
		listeContratsProd = @contratsProduits.getContratsFromClient(nomclient_brut)
		listePropositionsExp = @propositionsExpertise.getPropositionsFromClient(nomclient_brut)
		listePropositionsSup = @propositionsSupport.getPropositionsFromClient(nomclient_brut)
		
		unless @sqlQuery.nil? then
			@sqlQuery.deleteClient(raison_sociale, div, dep)
			
			@sqlQuery.deleteContracts(Config::EXPERTISE,listeContratsExp) 
			@sqlQuery.deleteContracts(Config::SUPPORT,listeContratsSup)
			@sqlQuery.deleteContracts(Config::PRODUIT,listeContratsProd)
			@sqlQuery.deletePropositions(Config::EXPERTISE,listePropositionsExp)
			@sqlQuery.deletePropositions(Config::SUPPORT,listePropositionsSup)
		else
			puts "SqlQuery wasn't created : database update cannot be performed"
		end
		@clients.delClient(raison_sociale,div,dep)
	
		@contratsExpertise.deleteContracts(listeContratsExp)
		@contratsSupport.deleteContracts(listeContratsSup)
		@contratsProduits.deleteContracts(listeContratsProd)
		@propositionsExpertise.deletePropositions(listePropositionsExp)
		@propositionsSupport.deletePropositions(listePropositionsSup)
	end
end

  # Mixin module for HTML version 3 generation methods.
  module Html3 # :nodoc:

    # The DOCTYPE declaration for this version of HTML
    def doctype
      %|<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">|
    end

    # Initialise the HTML generation methods for this version.
    def element_init
      extend TagMaker
      methods = ""
      # - -
      for element in %w[ A TT I B U STRIKE BIG SMALL SUB SUP EM STRONG
          DFN CODE SAMP KBD VAR CITE FONT ADDRESS DIV center MAP
          APPLET PRE XMP LISTING DL OL UL DIR MENU SELECT table TITLE
          STYLE SCRIPT H1 H2 H3 H4 H5 H6 TEXTAREA FORM BLOCKQUOTE
          CAPTION ]
        methods += <<-BEGIN + nn_element_def(element) + <<-END
          def #{element.downcase}(attributes = {})
        BEGIN
          end
        END
      end

      # - O EMPTY
      for element in %w[ IMG BASE BASEFONT BR AREA LINK PARAM HR INPUT
          ISINDEX META ]
        methods += <<-BEGIN + nOE_element_def(element) + <<-END
          def #{element.downcase}(attributes = {})
        BEGIN
          end
        END
      end

      # O O or - O
      for element in %w[ HTML HEAD BODY P PLAINTEXT DT DD LI OPTION tr
          th td ]
        methods += <<-BEGIN + nO_element_def(element) + <<-END
          def #{element.downcase}(attributes = {})
        BEGIN
          end
        END
      end
      eval(methods)
    end

  end

# following snippet from Webrick's log.rb
# notice the erronous handling of the query method is_a?
def format(arg)
     str = if arg.is_a?(Exception)
        "#{arg.class}: #{arg.message}\n\t" <<
        arg.backtrace.join("\n\t") << "\n"
      elsif arg.respond_to?(:to_str)
        arg.to_str
      else
        arg.inspect
      end
end

# following snippet from Webrick's httputils.rb
# Splitting regexps on several lines might be bad form,
# but not illegal in Ruby. 
# This should probably be supported in the highlighting
def split_header_value(str)
      str.scan(/((?:"(?:\\.|[^"])+?"|[^",]+)+)
                (?:,\s*|\Z)/xn).collect{|v| v[0] }
end

# snippet from Net::Telnet
string.gsub(/#{IAC}(
                   [#{IAC}#{AO}#{AYT}#{DM}#{IP}#{NOP}]|
                   [#{DO}#{DONT}#{WILL}#{WONT}]
                     [#{OPT_BINARY}-#{OPT_NEW_ENVIRON}#{OPT_EXOPL}]|
                   #{SB}[^#{IAC}]*#{IAC}#{SE}
                 )/xno)

# following snippet from Webrick's httpresponse.rb
# the HEREDOC is not recognized as such
@body << <<-_end_of_html_
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN">
<HTML>
  <HEAD><TITLE>#{HTMLUtils::escape(@reason_phrase)}</TITLE></HEAD>
  <BODY>
    <H1>#{HTMLUtils::escape(@reason_phrase)}</H1>
    #{HTMLUtils::escape(ex.message)}
    <HR>
_end_of_html_


# snippet from Webrick's httpproxy.rb
# here we should make sure that the symbol definition ':' doesn't override
# the module operator '::'
Net::HTTP::version_1_2 if RUBY_VERSION < "1.7"

# snippet from Webrick's cookie.rb
# the beginning of the regexp is erronously highlighted like an operator
key, val = x.split(/=/,2)

# the following are division operators
# it's a bit tricky to keep the operator apart from the regexp
result = 8 / 4
result /= divisor

# 2008-06-01 regexp and division operator syntax has been fixed:
result = 8/4    # division
result = 8/foo  # division
result = /8/    # regexp
result = 8//4/  # division and regexp

10/10           # division
10/ 10          # division
10 /10          # division
10 / 10         # division

foo/10          # division
foo/ 10         # division
foo /10/        # regexp
foo / 10        # division

foo/10/10       # both division
total/count/2   # both division
total/(count/2) # both division

@foo/10         # division
@foo /10        # division

"hello"/10      # division
"hello" / 10    # division

/regexp//10     # division
/regexp/ / 10   # division

Math::PI/10     # division
Math::foo /rx/  # regexp

# 2008-06-05 similar fix for modulo operator:

10%4            # modulo
10 %4           # modulo
10% 4           # modulo
10 % 4          # modulo

foo%4           # modulo
# foo %4          # illegal %string
foo% 4          # modulo
foo % 4         # modulo

foo % (4)       # modulo

foo %(4)        # %string
foo %q(4)       # %string
foo %Q(4)       # %string
foo %%4%        # %string

foo = %|blah|   # GDL input
foo % %|blah|   # modulo and GDL

# mix in any way you want
result = 10//regexp//20/foo//regexp//20

# test cases for general delimited input
# quoted strings
%Q|this is a string|
%Q{this is a string}
%Q(this is a string)
%Q<this is a string>
%Q[this is a string]

%|also a string|
%{also a string}
%(also a string)
%<also a string>
%[also a string]

# apostrophed strings
%q|apostrophed|
%q{apostrophed}
%q(apostrophed)
%q<apostrophed>
%q[apostrophed]

# regular expressions
%r{expression}
%r(expression)
%r<expression>
%r[expression]
%r|expression|

# shell commands
%x{ls -l}
%x(ls -l)
%x<ls -l>
%x[ls -l]

# sometimes it's useful to have the command on multiple lines
%x{ls -l |
grep test }

# alternative syntax
`ls -l`
`echo ' '`

# token array
%w{one two three}
%w(one two three)
%w<one two three>
%w[one two three]

# snippet from Net::IMAP
# I object to putting String, Integer and Array into kernel methods.
# While these classes are builtin in Ruby, this is an implementation detail
# that should not be exposed to the user.
# If we want to handle all std-lib classes, fine. But then they should be in their
# own std-lib keyword category.

def send_data(data)
      case data
      when nil
        put_string("NIL")
      when String
        send_string_data(data)
      when Integer
        send_number_data(data)
      when Array
        send_list_data(data)
      when Time
        send_time_data(data)
      when Symbol
        send_symbol_data(data)
      else
        data.send_data(self)
      end
end

# snippet from Net::POP
# class names can have numbers in them as long as they don't begin with numbers
# Ruby doesn't internally really make much of a difference between a class name and a constant

# class aliases
  POP = POP3
  POPSession  = POP3
  POP3Session = POP3

# "member access"
POP::Session.COUNT.attribute.calc_with(2){ |arg| puts arg }

# snippet from Net::SMTP
# This breaks the code folding. I think we would need to
# handle the '\' that continues the statement to the next line
# in some way to make these kind of things not break something.
raise ArgumentError, 'both user and secret are required'\
                      unless user and secret
 
# string escapes must detect escaping the escape char
str = "\\"
str = "\\\\"
# this is not part of the string
%x{echo \\\}\\}	# prints \}\
# this is not part of the command

# these are all symbols
:abc
:abc!
:abc?
:abc=
:[]
:[]=
:@abc
:@@abc
:$abc

# squiggly HEREDOC
    <<~HEREDOC
      Hello!!
    HEREDOC

# HEREDOC with backticks
<<`HEREDOC`
    echo "hello"
HEREDOC

# do not highlight HEREDOC markers after the "class" keyword
# (singleton class definition) (bug: #358273)
class <<Foo = Object.new
  attr_accessor :foo
end
singleton_class = ( class <<foo; self; end )

# highlight regular expressions after ": " (bug: #361875)
get 'files/:slug/:filename', to: 'files#download', slug: /^[a-z]+$/, filename: %r|^[/\s]+$|
@@hello!: /regexp/
[]=: %r!regexp!
