<?xml version="1.0" encoding="iso-8859-15"?>

<!--
	This file is freely distributable, created by Wilbert Berendsen (wbsoft@xs4all.nl)

	This is just a simple XSLT file that converts some HTML pages to a XBEL bookmarklist.
	It is included here to test the new XSLT highlighting by Peter Lammich.
	
	TODO: add better test file.
-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
xmlns:h="http://www.w3.org/1999/xhtml">
<xsl:output method="xml" encoding="iso-8859-15" indent="yes"
 doctype-public="+//IDN python.org//DTD XML Bookmark Exchange Language 1.0//EN//XML"
 doctype-system="http://www.python.org/topics/xml/dtds/xbel-1.0.dtd"
/>

<!-- main -->
<xsl:template match="/">
<xbel><folder><title>Linux at Home Links</title>
<xsl:for-each select="//*[@id='maincontents']//h:li">
  <xsl:variable name="f" select="document(h:a/@href)/h:html"/>
  <folder>
  <title><xsl:value-of select="h:a/h:strong"/></title>
  <desc><xsl:value-of select="normalize-space(h:a/text())"/></desc>
  <xsl:for-each select="$f//h:div[@id='maincontents']">
  <xsl:call-template name="getbookmarks"/>
  </xsl:for-each>
  
  </folder>
</xsl:for-each>
</folder></xbel>
</xsl:template>
<!-- end of main -->

<!-- get bookmarks from a page -->
<xsl:template name="getbookmarks">

<xsl:choose>
  <!-- harvest links from p or li elements -->
  <xsl:when test="self::h:li or self::h:p">
    <xsl:variable name="t" select="normalize-space()"/>
    <xsl:for-each select=".//h:a[1]">
      <xsl:call-template name="bookmark">
        <xsl:with-param name="desc" select="$t"/>
      </xsl:call-template>
    </xsl:for-each>
    <xsl:for-each select=".//h:a[position()!=1]">
      <xsl:call-template name="bookmark"/>
    </xsl:for-each>
  </xsl:when>


  <xsl:otherwise>
    <xsl:for-each select="*">
      <xsl:call-template name="getbookmarks"/>
    </xsl:for-each>
  </xsl:otherwise>

</xsl:choose>

</xsl:template>


<xsl:template name="bookmark">
  <xsl:param name="href" select="@href"/>
  <xsl:param name="desc" select="''"/>
  <xsl:param name="title" select="normalize-space()"/>

  <xsl:variable name="realdesc">
    <xsl:choose>
      <xsl:when test="starts-with($desc,$title)">
        <xsl:choose>
          <xsl:when test="starts-with(translate(substring-after($desc,$title),',.;','...'),'.')">
            <xsl:value-of select="substring($desc,string-length($title) + 2)"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="substring-after($desc,$title)"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$desc"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  
  <bookmark>
    <xsl:attribute name="href">
      <xsl:choose>
        <xsl:when test="starts-with($href,'http://') or starts-with($href,'ftp://') or starts-with($href,'mailto:')">
          <xsl:value-of select="$href"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="concat('http://www.xs4all.nl/~wbsoft/linux/links/',$href)"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
    <title><xsl:value-of select="normalize-space($title)"/></title>
    <xsl:if test="normalize-space($realdesc) != ''">
      <desc><xsl:value-of select="normalize-space($realdesc)"/></desc>
    </xsl:if>
  </bookmark>
</xsl:template>



</xsl:stylesheet>
