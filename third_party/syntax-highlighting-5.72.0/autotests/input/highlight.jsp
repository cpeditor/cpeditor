<%--
  This page won't actually work, as it is simply designed to display jsp syntax highlighting.
--%>
<%@ page info="A Page to Test Kate Jsp Syntax Highlighting" language="java" errorPage="/test-error-page.jsp"%>
<%@ include file="/include/myglobalvars.jsp"%> --%>
<%@ page import="java.util.*,
                 java.io.*,
                 java.math.*" %>
<%@ taglib uri="/WEB-INF/lib/si_taglib.tld" prefix="si"%>
<jsp:useBean id="aPageBean" scope="page" class="my.package.MyPageBean"/>
<jsp:useBean id="aRequestBean" scope="request" class="my.package.MyRequestBean"/>
<%
  // We can decipher our expected parameters here.
  String parm1 = noNull(request.getParameter(PARAMETER_1)).trim();
  String parm2 = noNull(request.getParameter(PARAMETER_2)).trim();
  String parm3 = noNull(request.getParameter(PARAMETER_3)).trim();
  String parm4 = noNull(request.getParameter(PARAMETER_4)).trim();
  String parm5 = noNull(request.getParameter(PARAMETER_5)).trim();

  // A sample collection of Integers to display some code folding.
  List intList = getIntList(10);


%>
<html>
  <title>A Sample Jsp</title>
  <head>
  <script language="javascript"><!--
    function doAlert1() {
      alert("This is the first javascript example.");
    }

    function doAlert2() {
      alert("This is the second javascript example.");
    }
  //--></script>
  <style type="text/css">
    body{ color: yellow; }
  </style>
  </head>
  <body>
    <%-- The top label table. --%>
    <table width="400" cellpadding="0" cellspacing="0" border="0">
      <tr>
        <td><font size="3"><b>The following parameters were detected:</b></font></td>
      </tr>
    </table>

    <%-- Display the parameters which might have been passed in. --%>
    <table width="400" cellpadding="0" cellspacing="0" border="0">
      <%-- Label; Actual Parameter String; Value Detected --%>
      <tr>
        <td><b>PARAMETER_1</b></td>
        <td align="center"><%=PARAMETER_1%></td>
        <td align="right">&quot;<%=parm1%>&quot;</td>
      </tr>

      <%-- Label; Actual Parameter String; Value Detected --%>
      <tr>
        <td><b>PARAMETER_2</b></td>
        <td align="center"><%=PARAMETER_2%></td>
        <td align="right">&quot;<%=parm2%>&quot;</td>
      </tr>

      <%-- Label; Actual Parameter String; Value Detected --%>
      <tr>
        <td><b>PARAMETER_3</b></td>
        <td align="center"><%=PARAMETER_3%></td>
        <td align="right">&quot;<%=parm3%>&quot;</td>
      </tr>

      <%-- Label; Actual Parameter String; Value Detected --%>
      <tr>
        <td><b>PARAMETER_4</b></td>
        <td align="center"><%=PARAMETER_4%></td>
        <td align="right">&quot;<%=parm4%>&quot;</td>
      </tr>

      <%-- Label; Actual Parameter String; Value Detected --%>
      <tr>
        <td><b>PARAMETER_5</b></td>
        <td align="center"><%=PARAMETER_5%></td>
        <td align="right">&quot;<%=parm5%>&quot;</td>
      </tr>
    </table>

    <br><br>

    <%-- Display our list of random Integers (shows code folding). --%>
    <table width="400" cellpadding="0" cellspacing="0" border="0">
<%
  if (intList != null && intList.size() > 0) {
%>
      <tr><td><b>Here are the elements of intList...</b></td></tr>
<%
    Iterator intListIt = intList.iterator();
    while (intListIt.hasNext()) {
      Integer i = (Integer) intListIt.next();
%>
      <tr><td><%=i.toString()%></td></tr>
<%
    }
  } else {
%>
      <tr><td><font color="blue"><b><i>Oooops, we forgot to initialize intList!</i></b></font></td></tr>
<%
  }
%>
    </table>

    <br><br>

    <%-- We can call javascript functions. --%>
    <table width="400" cellpadding="0" cellspacing="0" border="0">
      <tr><td colspan="2"><b>Test our javascript...</b></td></tr>
      <tr>
        <td><input type="button" name="button1" value="Alert 1" onmouseup="javascript:doAlert1()"></td>
        <td><input type="button" name="button2" value="Alert 2" onmouseup="javascript:doAlert2()"></td>
      </tr>
    </table>

    <br><br>
    <%-- If we actually had defined a tag library. --%>
    <table width="400" cellpadding="0" cellspacing="0" border="0">
      <tr><td>
      <my:SampleTag prop1="first" prop2="third">
        <my:SampleTagChild nameProp="value1"/>
        <my:SampleTagChild nameProp="value2"/>
      </my:SampleTag>
      </td></tr>
    </table>

    <br><br>
    <%-- Expression language. --%>
    <table width="400" cellpadding="0" cellspacing="0" border="0">
      <c:if test="${!empty param.aParam}">
        <c:set var="myParam" scope="session" value="${param.aParam}"/>
      </c:if>

      <tr><td>myParam's value: &quot;<c:out value="${myParam}" default=="Default"/>&quot;</td></tr>
    </table>
  </body>
</html>
<%!
  /* A place for class variables and functions... */

  // Define some sample parameter names that this page might understand.
  private static final String PARAMETER_1            = "p1";
  private static final String PARAMETER_2            = "p2";
  private static final String PARAMETER_3            = "p3";
  private static final String PARAMETER_4            = "p4";
  private static final String PARAMETER_5            = "p5";

  // Returns str trimmed, or an empty string if str is null.
  private static String noNull(String str) {
    String retStr;
    if (str == null)
      retStr = "";
    else
      retStr = str.trim();

    return retStr;
  }

  // Returns a list of Integers with listSize elements.
  private static List getIntList(int listSize) {
    ArrayList retList = new ArrayList(listSize);
    for (int i = 0; i < listSize; i++)
      retList.add(new Integer( (int) (Math.random() * 100) ));

    return retList;
  }
%>
