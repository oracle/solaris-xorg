<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version='1.0'>
  <!-- settings similar to those xmlto would use if we had it available -->
  <xsl:import href="/usr/share/sgml/X11/xorg.xsl"/>
  <xsl:import href="/usr/share/sgml/X11/xorg-xhtml.xsl"/>
  <xsl:param name="chunker.output.encoding" select="'UTF-8'"/>
  <xsl:param name="man.charmap.use.subset" select="'0'"/>
</xsl:stylesheet>
