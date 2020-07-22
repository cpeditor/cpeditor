<% 'kate: hl ASP;
if ( instr(request.servervariables("PATH_INFO"),"login.asp") <= 0 and instr(request.servervariables("PATH_INFO"),"inset") <= 0 and instr(request.servervariables("PATH_INFO"),"Data") <= 0 and instr(request.servervariables("PATH_INFO"),"dropDown") <= 0 ) then
	Session("originalRequestedPage") = Request.ServerVariables("PATH_INFO") & "?" & Request.ServerVariables("QUERY_STRING")
end if

function countRecords( rsToCount )
	numRecs = 0

	do until rsToCount.eof
		numRecs = numRecs + 1

		rsToCount.movenext
	loop

	rsToCount.close ' just to make sure nobody
		' tries to operate on the recordset,
		' which has already reached eof

	countRecords = numRecs
end function

function unique( rs, sortColumn ) ' return unique instances of text in sortColumn within rs
	dim sorted()

	redim sorted(1)
	dim i
	i = 0
	do until rs.eof
		if (not find( rs(sortColumn), sorted )) then
			redim preserve sorted(i+1)
			sorted(i) = rs(sortColumn)
		 	i = i + 1
		end if
		rs.MoveNext
	loop

	redim preserve sorted(i-1) ' the function will add an extra blank entry to the array

	rs.Close 	' close the recordset - we'll be using it again - and reset i - well be using it again, too

	unique = sorted
end function

sub testSub( variable ) ' do nothing impressive...
	dim newVar

	newVar = variable

	if ( variable = true )
		response.end
	else %>

<p>We are writing text.</p>
<p class="stuff"><%=newVar%></p>
<p>We have written text and outputted a variable.</p>

<% 	end if
end sub %>