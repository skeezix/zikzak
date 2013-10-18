<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.4">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="1" fill="3" visible="no" active="no"/>
<layer number="3" name="Route3" color="4" fill="3" visible="no" active="no"/>
<layer number="4" name="Route4" color="1" fill="4" visible="no" active="no"/>
<layer number="5" name="Route5" color="4" fill="4" visible="no" active="no"/>
<layer number="6" name="Route6" color="1" fill="8" visible="no" active="no"/>
<layer number="7" name="Route7" color="4" fill="8" visible="no" active="no"/>
<layer number="8" name="Route8" color="1" fill="2" visible="no" active="no"/>
<layer number="9" name="Route9" color="4" fill="2" visible="no" active="no"/>
<layer number="10" name="Route10" color="1" fill="7" visible="no" active="no"/>
<layer number="11" name="Route11" color="4" fill="7" visible="no" active="no"/>
<layer number="12" name="Route12" color="1" fill="5" visible="no" active="no"/>
<layer number="13" name="Route13" color="4" fill="5" visible="no" active="no"/>
<layer number="14" name="Route14" color="1" fill="6" visible="no" active="no"/>
<layer number="15" name="Route15" color="4" fill="6" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="zilog">
<description>&lt;b&gt;Zilog Z80 Microprocessor Devices&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="DIL40">
<description>&lt;b&gt;Dual In Line Package&lt;/b&gt;</description>
<wire x1="25.4" y1="6.731" x2="-25.4" y2="6.731" width="0.1524" layer="21"/>
<wire x1="-25.4" y1="-6.731" x2="25.4" y2="-6.731" width="0.1524" layer="21"/>
<wire x1="25.4" y1="6.731" x2="25.4" y2="-6.731" width="0.1524" layer="21"/>
<wire x1="-25.4" y1="6.731" x2="-25.4" y2="0.889" width="0.1524" layer="21"/>
<wire x1="-25.4" y1="-6.731" x2="-25.4" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="-25.4" y1="0.889" x2="-25.4" y2="-1.143" width="0.1524" layer="21" curve="-180"/>
<pad name="1" x="-24.13" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="2" x="-21.59" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="7" x="-8.89" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="8" x="-6.35" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="3" x="-19.05" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="4" x="-16.51" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="6" x="-11.43" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="5" x="-13.97" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="9" x="-3.81" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="10" x="-1.27" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="11" x="1.27" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="12" x="3.81" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="13" x="6.35" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="14" x="8.89" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="15" x="11.43" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="16" x="13.97" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="17" x="16.51" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="18" x="19.05" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="19" x="21.59" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="20" x="24.13" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="21" x="24.13" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="22" x="21.59" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="23" x="19.05" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="24" x="16.51" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="25" x="13.97" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="26" x="11.43" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="27" x="8.89" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="28" x="6.35" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="29" x="3.81" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="30" x="1.27" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="31" x="-1.27" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="32" x="-3.81" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="33" x="-6.35" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="34" x="-8.89" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="35" x="-11.43" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="36" x="-13.97" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="37" x="-16.51" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="38" x="-19.05" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="39" x="-21.59" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="40" x="-24.13" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<text x="-25.908" y="-6.604" size="1.778" layer="25" rot="R90">&gt;NAME</text>
<text x="-17.145" y="-1.016" size="1.778" layer="27">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="Z80CPU">
<wire x1="-12.7" y1="-35.56" x2="12.7" y2="-35.56" width="0.4064" layer="94"/>
<wire x1="12.7" y1="30.48" x2="12.7" y2="-35.56" width="0.4064" layer="94"/>
<wire x1="12.7" y1="30.48" x2="-12.7" y2="30.48" width="0.4064" layer="94"/>
<wire x1="-12.7" y1="-35.56" x2="-12.7" y2="30.48" width="0.4064" layer="94"/>
<text x="-12.7" y="31.115" size="1.778" layer="95">&gt;NAME</text>
<text x="-12.7" y="-38.1" size="1.778" layer="96">&gt;VALUE</text>
<pin name="A0" x="17.78" y="27.94" length="middle" direction="out" rot="R180"/>
<pin name="A1" x="17.78" y="25.4" length="middle" direction="out" rot="R180"/>
<pin name="A2" x="17.78" y="22.86" length="middle" direction="out" rot="R180"/>
<pin name="A3" x="17.78" y="20.32" length="middle" direction="out" rot="R180"/>
<pin name="A4" x="17.78" y="17.78" length="middle" direction="out" rot="R180"/>
<pin name="A5" x="17.78" y="15.24" length="middle" direction="out" rot="R180"/>
<pin name="A6" x="17.78" y="12.7" length="middle" direction="out" rot="R180"/>
<pin name="A7" x="17.78" y="10.16" length="middle" direction="out" rot="R180"/>
<pin name="A8" x="17.78" y="7.62" length="middle" direction="out" rot="R180"/>
<pin name="A9" x="17.78" y="5.08" length="middle" direction="out" rot="R180"/>
<pin name="A10" x="17.78" y="2.54" length="middle" direction="out" rot="R180"/>
<pin name="A11" x="17.78" y="0" length="middle" direction="out" rot="R180"/>
<pin name="A12" x="17.78" y="-2.54" length="middle" direction="out" rot="R180"/>
<pin name="A13" x="17.78" y="-5.08" length="middle" direction="out" rot="R180"/>
<pin name="A14" x="17.78" y="-7.62" length="middle" direction="out" rot="R180"/>
<pin name="A15" x="17.78" y="-10.16" length="middle" direction="out" rot="R180"/>
<pin name="D0" x="17.78" y="-15.24" length="middle" rot="R180"/>
<pin name="D1" x="17.78" y="-17.78" length="middle" rot="R180"/>
<pin name="D2" x="17.78" y="-20.32" length="middle" rot="R180"/>
<pin name="D3" x="17.78" y="-22.86" length="middle" rot="R180"/>
<pin name="D4" x="17.78" y="-25.4" length="middle" rot="R180"/>
<pin name="D5" x="17.78" y="-27.94" length="middle" rot="R180"/>
<pin name="D6" x="17.78" y="-30.48" length="middle" rot="R180"/>
<pin name="D7" x="17.78" y="-33.02" length="middle" rot="R180"/>
<pin name="M1" x="-17.78" y="27.94" length="middle" direction="out" function="dot"/>
<pin name="MREQ" x="-17.78" y="25.4" length="middle" direction="out" function="dot"/>
<pin name="IORQ" x="-17.78" y="22.86" length="middle" direction="out" function="dot"/>
<pin name="RD" x="-17.78" y="20.32" length="middle" direction="out" function="dot"/>
<pin name="WR" x="-17.78" y="17.78" length="middle" direction="out" function="dot"/>
<pin name="RFSH" x="-17.78" y="12.7" length="middle" direction="out" function="dot"/>
<pin name="HALT" x="-17.78" y="7.62" length="middle" direction="out" function="dot"/>
<pin name="WAIT" x="-17.78" y="2.54" length="middle" direction="in" function="dot"/>
<pin name="INT" x="-17.78" y="-2.54" length="middle" direction="in" function="dot"/>
<pin name="NMI" x="-17.78" y="-5.08" length="middle" direction="in" function="dot"/>
<pin name="RESET" x="-17.78" y="-10.16" length="middle" direction="in" function="dot"/>
<pin name="BUSRQ" x="-17.78" y="-15.24" length="middle" direction="in" function="dot"/>
<pin name="BUSAK" x="-17.78" y="-17.78" length="middle" direction="out" function="dot"/>
<pin name="CLK" x="-17.78" y="-22.86" length="middle" direction="in" function="clk"/>
<pin name="VCC" x="-17.78" y="-33.02" length="middle" direction="pwr"/>
<pin name="GND" x="-17.78" y="-27.94" length="middle" direction="pwr"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="Z80CPU" prefix="IC" uservalue="yes">
<description>&lt;b&gt;Z80 FAMILY DEVICE&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="Z80CPU" x="0" y="0"/>
</gates>
<devices>
<device name="" package="DIL40">
<connects>
<connect gate="1" pin="A0" pad="30"/>
<connect gate="1" pin="A1" pad="31"/>
<connect gate="1" pin="A10" pad="40"/>
<connect gate="1" pin="A11" pad="1"/>
<connect gate="1" pin="A12" pad="2"/>
<connect gate="1" pin="A13" pad="3"/>
<connect gate="1" pin="A14" pad="4"/>
<connect gate="1" pin="A15" pad="5"/>
<connect gate="1" pin="A2" pad="32"/>
<connect gate="1" pin="A3" pad="33"/>
<connect gate="1" pin="A4" pad="34"/>
<connect gate="1" pin="A5" pad="35"/>
<connect gate="1" pin="A6" pad="36"/>
<connect gate="1" pin="A7" pad="37"/>
<connect gate="1" pin="A8" pad="38"/>
<connect gate="1" pin="A9" pad="39"/>
<connect gate="1" pin="BUSAK" pad="23"/>
<connect gate="1" pin="BUSRQ" pad="25"/>
<connect gate="1" pin="CLK" pad="6"/>
<connect gate="1" pin="D0" pad="14"/>
<connect gate="1" pin="D1" pad="15"/>
<connect gate="1" pin="D2" pad="12"/>
<connect gate="1" pin="D3" pad="8"/>
<connect gate="1" pin="D4" pad="7"/>
<connect gate="1" pin="D5" pad="9"/>
<connect gate="1" pin="D6" pad="10"/>
<connect gate="1" pin="D7" pad="13"/>
<connect gate="1" pin="GND" pad="29"/>
<connect gate="1" pin="HALT" pad="18"/>
<connect gate="1" pin="INT" pad="16"/>
<connect gate="1" pin="IORQ" pad="20"/>
<connect gate="1" pin="M1" pad="27"/>
<connect gate="1" pin="MREQ" pad="19"/>
<connect gate="1" pin="NMI" pad="17"/>
<connect gate="1" pin="RD" pad="21"/>
<connect gate="1" pin="RESET" pad="26"/>
<connect gate="1" pin="RFSH" pad="28"/>
<connect gate="1" pin="VCC" pad="11"/>
<connect gate="1" pin="WAIT" pad="24"/>
<connect gate="1" pin="WR" pad="22"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="skeezix">
<description>&lt;b&gt;IC Packages an Sockets&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="DIL24-3">
<description>&lt;b&gt;Dual In Line Package&lt;/b&gt; 0.3 inch</description>
<wire x1="-14.986" y1="-0.635" x2="-14.986" y2="-2.794" width="0.1524" layer="21"/>
<wire x1="-14.986" y1="0.635" x2="-14.986" y2="-0.635" width="0.1524" layer="21" curve="-180"/>
<wire x1="14.986" y1="-2.794" x2="14.986" y2="2.794" width="0.1524" layer="21"/>
<wire x1="-14.986" y1="-2.794" x2="14.986" y2="-2.794" width="0.1524" layer="21"/>
<wire x1="-14.986" y1="2.794" x2="14.986" y2="2.794" width="0.1524" layer="21"/>
<wire x1="-14.986" y1="2.794" x2="-14.986" y2="0.635" width="0.1524" layer="21"/>
<pad name="1" x="-13.97" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="2" x="-11.43" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="3" x="-8.89" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="4" x="-6.35" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="5" x="-3.81" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="6" x="-1.27" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="7" x="1.27" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="8" x="3.81" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="9" x="6.35" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="10" x="8.89" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="11" x="11.43" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="12" x="13.97" y="-3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="13" x="13.97" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="14" x="11.43" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="15" x="8.89" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="16" x="6.35" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="17" x="3.81" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="18" x="1.27" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="19" x="-1.27" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="20" x="-3.81" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="21" x="-6.35" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="22" x="-8.89" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="23" x="-11.43" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<pad name="24" x="-13.97" y="3.81" drill="0.8128" shape="long" rot="R90"/>
<text x="-15.2908" y="-2.667" size="1.778" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="-13.462" y="-0.889" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="DIL24-4">
<description>&lt;b&gt;Dual In Line Package&lt;/b&gt; 0.4 inch</description>
<wire x1="-15.367" y1="-0.635" x2="-15.367" y2="-4.064" width="0.1524" layer="21"/>
<wire x1="-15.367" y1="0.635" x2="-15.367" y2="-0.635" width="0.1524" layer="21" curve="-180"/>
<wire x1="15.367" y1="-4.064" x2="15.367" y2="4.064" width="0.1524" layer="21"/>
<wire x1="-15.367" y1="-4.064" x2="15.367" y2="-4.064" width="0.1524" layer="21"/>
<wire x1="-15.367" y1="4.064" x2="-15.367" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-15.367" y1="4.064" x2="15.367" y2="4.064" width="0.1524" layer="21"/>
<pad name="1" x="-13.97" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="2" x="-11.43" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="3" x="-8.89" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="4" x="-6.35" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="5" x="-3.81" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="6" x="-1.27" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="7" x="1.27" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="8" x="3.81" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="9" x="6.35" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="10" x="8.89" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="11" x="11.43" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="12" x="13.97" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="13" x="13.97" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="14" x="11.43" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="15" x="8.89" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="16" x="6.35" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="17" x="3.81" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="18" x="1.27" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="19" x="-1.27" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="20" x="-3.81" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="21" x="-6.35" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="22" x="-8.89" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="23" x="-11.43" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="24" x="-13.97" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<text x="-15.6464" y="-3.81" size="1.778" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="-12.7" y="-0.8382" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="DIL24-6">
<description>&lt;b&gt;Dual In Line Package&lt;/b&gt; 0.6 inch</description>
<wire x1="-15.113" y1="-1.27" x2="-15.113" y2="-6.604" width="0.1524" layer="21"/>
<wire x1="-15.113" y1="1.27" x2="-15.113" y2="-1.27" width="0.1524" layer="21" curve="-180"/>
<wire x1="15.113" y1="-6.604" x2="15.113" y2="6.604" width="0.1524" layer="21"/>
<wire x1="-15.113" y1="6.604" x2="-15.113" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-15.113" y1="6.604" x2="15.113" y2="6.604" width="0.1524" layer="21"/>
<wire x1="-15.113" y1="-6.604" x2="15.113" y2="-6.604" width="0.1524" layer="21"/>
<pad name="1" x="-13.97" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="2" x="-11.43" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="3" x="-8.89" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="4" x="-6.35" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="5" x="-3.81" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="6" x="-1.27" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="7" x="1.27" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="8" x="3.81" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="9" x="6.35" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="10" x="8.89" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="11" x="11.43" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="12" x="13.97" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="13" x="13.97" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="14" x="11.43" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="15" x="8.89" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="16" x="6.35" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="17" x="3.81" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="18" x="1.27" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="19" x="-1.27" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="20" x="-3.81" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="21" x="-6.35" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="22" x="-8.89" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="23" x="-11.43" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="24" x="-13.97" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<text x="-15.621" y="-6.35" size="1.778" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="-12.065" y="-0.889" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="DIL24-9">
<description>&lt;b&gt;Dual In Line Package&lt;/b&gt; 0.9 inch</description>
<wire x1="-16.129" y1="-1.27" x2="-16.129" y2="-10.414" width="0.1524" layer="21"/>
<wire x1="-16.129" y1="1.27" x2="-16.129" y2="-1.27" width="0.1524" layer="21" curve="-180"/>
<wire x1="16.129" y1="-10.414" x2="16.129" y2="10.414" width="0.1524" layer="21"/>
<wire x1="-16.129" y1="10.414" x2="-16.129" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-16.129" y1="10.414" x2="16.129" y2="10.414" width="0.1524" layer="21"/>
<wire x1="-16.129" y1="-10.414" x2="16.129" y2="-10.414" width="0.1524" layer="21"/>
<pad name="1" x="-13.97" y="-11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="2" x="-11.43" y="-11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="3" x="-8.89" y="-11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="4" x="-6.35" y="-11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="5" x="-3.81" y="-11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="6" x="-1.27" y="-11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="7" x="1.27" y="-11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="8" x="3.81" y="-11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="9" x="6.35" y="-11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="10" x="8.89" y="-11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="11" x="11.43" y="-11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="12" x="13.97" y="-11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="13" x="13.97" y="11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="14" x="11.43" y="11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="15" x="8.89" y="11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="16" x="6.35" y="11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="17" x="3.81" y="11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="18" x="1.27" y="11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="19" x="-1.27" y="11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="20" x="-3.81" y="11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="21" x="-6.35" y="11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="22" x="-8.89" y="11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="23" x="-11.43" y="11.43" drill="0.8128" shape="long" rot="R90"/>
<pad name="24" x="-13.97" y="11.43" drill="0.8128" shape="long" rot="R90"/>
<text x="-16.51" y="-9.906" size="1.778" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="-11.43" y="-0.635" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="DIL28-4">
<description>&lt;b&gt;Dual In Line Package&lt;/b&gt;</description>
<wire x1="17.78" y1="4.191" x2="-17.78" y2="4.191" width="0.1524" layer="21"/>
<wire x1="-17.78" y1="-4.191" x2="17.78" y2="-4.191" width="0.1524" layer="21"/>
<wire x1="17.78" y1="4.191" x2="17.78" y2="-4.191" width="0.1524" layer="21"/>
<wire x1="-17.78" y1="4.191" x2="-17.78" y2="1.016" width="0.1524" layer="21"/>
<wire x1="-17.78" y1="-4.191" x2="-17.78" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="-17.78" y1="1.016" x2="-17.78" y2="-1.016" width="0.1524" layer="21" curve="-180"/>
<pad name="1" x="-16.51" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="2" x="-13.97" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="7" x="-1.27" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="8" x="1.27" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="3" x="-11.43" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="4" x="-8.89" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="6" x="-3.81" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="5" x="-6.35" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="9" x="3.81" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="10" x="6.35" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="11" x="8.89" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="12" x="11.43" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="13" x="13.97" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="14" x="16.51" y="-5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="15" x="16.51" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="16" x="13.97" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="17" x="11.43" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="18" x="8.89" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="19" x="6.35" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="20" x="3.81" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="21" x="1.27" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="22" x="-1.27" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="23" x="-3.81" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="24" x="-6.35" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="25" x="-8.89" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="26" x="-11.43" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="27" x="-13.97" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<pad name="28" x="-16.51" y="5.08" drill="0.8128" shape="long" rot="R90"/>
<text x="-18.161" y="-4.191" size="1.778" layer="25" rot="R90">&gt;NAME</text>
<text x="-13.335" y="-1.016" size="1.778" layer="27">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="DIL24">
<wire x1="-5.08" y1="16.51" x2="-5.08" y2="-13.97" width="0.254" layer="94"/>
<wire x1="-5.08" y1="-13.97" x2="5.08" y2="-13.97" width="0.254" layer="94"/>
<wire x1="5.08" y1="-13.97" x2="5.08" y2="16.51" width="0.254" layer="94"/>
<wire x1="5.08" y1="16.51" x2="2.54" y2="16.51" width="0.254" layer="94"/>
<wire x1="-5.08" y1="16.51" x2="-2.54" y2="16.51" width="0.254" layer="94"/>
<wire x1="-2.54" y1="16.51" x2="2.54" y2="16.51" width="0.254" layer="94" curve="180"/>
<text x="-4.445" y="17.145" size="1.778" layer="95">&gt;NAME</text>
<text x="-4.445" y="-16.51" size="1.778" layer="96">&gt;VALUE</text>
<pin name="1" x="-7.62" y="15.24" visible="pad" length="short" direction="pas"/>
<pin name="2" x="-7.62" y="12.7" visible="pad" length="short" direction="pas"/>
<pin name="3" x="-7.62" y="10.16" visible="pad" length="short" direction="pas"/>
<pin name="4" x="-7.62" y="7.62" visible="pad" length="short" direction="pas"/>
<pin name="5" x="-7.62" y="5.08" visible="pad" length="short" direction="pas"/>
<pin name="6" x="-7.62" y="2.54" visible="pad" length="short" direction="pas"/>
<pin name="7" x="-7.62" y="0" visible="pad" length="short" direction="pas"/>
<pin name="8" x="-7.62" y="-2.54" visible="pad" length="short" direction="pas"/>
<pin name="9" x="-7.62" y="-5.08" visible="pad" length="short" direction="pas"/>
<pin name="10" x="-7.62" y="-7.62" visible="pad" length="short" direction="pas"/>
<pin name="11" x="-7.62" y="-10.16" visible="pad" length="short" direction="pas"/>
<pin name="12" x="-7.62" y="-12.7" visible="pad" length="short" direction="pas"/>
<pin name="13" x="7.62" y="-12.7" visible="pad" length="short" direction="pas" rot="R180"/>
<pin name="14" x="7.62" y="-10.16" visible="pad" length="short" direction="pas" rot="R180"/>
<pin name="15" x="7.62" y="-7.62" visible="pad" length="short" direction="pas" rot="R180"/>
<pin name="16" x="7.62" y="-5.08" visible="pad" length="short" direction="pas" rot="R180"/>
<pin name="17" x="7.62" y="-2.54" visible="pad" length="short" direction="pas" rot="R180"/>
<pin name="18" x="7.62" y="0" visible="pad" length="short" direction="pas" rot="R180"/>
<pin name="19" x="7.62" y="2.54" visible="pad" length="short" direction="pas" rot="R180"/>
<pin name="20" x="7.62" y="5.08" visible="pad" length="short" direction="pas" rot="R180"/>
<pin name="21" x="7.62" y="7.62" visible="pad" length="short" direction="pas" rot="R180"/>
<pin name="22" x="7.62" y="10.16" visible="pad" length="short" direction="pas" rot="R180"/>
<pin name="23" x="7.62" y="12.7" visible="pad" length="short" direction="pas" rot="R180"/>
<pin name="24" x="7.62" y="15.24" visible="pad" length="short" direction="pas" rot="R180"/>
<text x="-4.572" y="15.24" size="0.8128" layer="94">Vss</text>
<text x="-4.572" y="12.954" size="0.8128" layer="94">Rx Data</text>
<text x="-4.572" y="10.668" size="0.8128" layer="94">Rx Clock</text>
<text x="-4.572" y="7.62" size="0.8128" layer="94">Tx Clock</text>
<text x="-4.572" y="5.334" size="0.8128" layer="94">/RTS</text>
<text x="-4.572" y="3.048" size="0.8128" layer="94">Tx Data</text>
<text x="-4.572" y="0" size="0.8128" layer="94">/IRQ</text>
<text x="-4.572" y="-2.286" size="0.8128" layer="94">CS0</text>
<text x="-4.572" y="-5.334" size="0.8128" layer="94">/CS2</text>
<text x="-4.572" y="-7.62" size="0.8128" layer="94">CS1</text>
<text x="-4.572" y="-9.906" size="0.8128" layer="94">RS</text>
<text x="-4.572" y="-12.192" size="0.8128" layer="94">Vcc</text>
<text x="2.286" y="14.478" size="0.8128" layer="94">/CTS</text>
<text x="1.524" y="12.192" size="0.8128" layer="94">/DCD</text>
<text x="3.048" y="9.906" size="0.8128" layer="94">D0</text>
<text x="3.048" y="7.62" size="0.8128" layer="94">D1</text>
<text x="3.048" y="5.334" size="0.8128" layer="94">D2</text>
<text x="3.048" y="2.286" size="0.8128" layer="94">D3</text>
<text x="3.048" y="0" size="0.8128" layer="94">D4</text>
<text x="3.048" y="-2.286" size="0.8128" layer="94">D5</text>
<text x="3.048" y="-4.572" size="0.8128" layer="94">D6</text>
<text x="3.048" y="-7.62" size="0.8128" layer="94">D7</text>
<text x="3.048" y="-9.906" size="0.8128" layer="94">E</text>
<text x="1.524" y="-12.954" size="0.8128" layer="94">R /W</text>
</symbol>
<symbol name="AT28C256">
<wire x1="0" y1="0" x2="0" y2="45.72" width="0.254" layer="94"/>
<wire x1="0" y1="45.72" x2="22.86" y2="45.72" width="0.254" layer="94"/>
<wire x1="22.86" y1="45.72" x2="22.86" y2="0" width="0.254" layer="94"/>
<wire x1="22.86" y1="0" x2="0" y2="0" width="0.254" layer="94"/>
<pin name="A14" x="-5.08" y="43.18" length="middle" direction="in"/>
<pin name="A12" x="-5.08" y="40.64" length="middle" direction="in"/>
<pin name="A7" x="-5.08" y="38.1" length="middle" direction="in"/>
<pin name="A6" x="-5.08" y="35.56" length="middle" direction="in"/>
<pin name="A5" x="-5.08" y="33.02" length="middle" direction="in"/>
<pin name="A4" x="-5.08" y="30.48" length="middle" direction="in"/>
<pin name="A3" x="-5.08" y="27.94" length="middle" direction="in"/>
<pin name="A2" x="-5.08" y="25.4" length="middle" direction="in"/>
<pin name="A1" x="-5.08" y="22.86" length="middle" direction="in"/>
<pin name="A0" x="-5.08" y="20.32" length="middle" direction="in"/>
<pin name="IO0" x="-5.08" y="15.24" length="middle"/>
<pin name="IO1" x="-5.08" y="12.7" length="middle"/>
<pin name="IO2" x="-5.08" y="10.16" length="middle"/>
<pin name="GND" x="-5.08" y="2.54" length="middle" direction="pwr"/>
<pin name="VCC" x="27.94" y="43.18" length="middle" direction="pwr" rot="R180"/>
<pin name="/WE" x="27.94" y="40.64" length="middle" direction="in" rot="R180"/>
<pin name="A13" x="27.94" y="35.56" length="middle" direction="in" rot="R180"/>
<pin name="A8" x="27.94" y="33.02" length="middle" direction="in" rot="R180"/>
<pin name="A9" x="27.94" y="30.48" length="middle" direction="in" rot="R180"/>
<pin name="A11" x="27.94" y="27.94" length="middle" direction="in" rot="R180"/>
<pin name="/OE" x="27.94" y="25.4" length="middle" direction="in" rot="R180"/>
<pin name="A10" x="27.94" y="22.86" length="middle" direction="in" rot="R180"/>
<pin name="/CE" x="27.94" y="20.32" length="middle" direction="in" rot="R180"/>
<pin name="IO7" x="27.94" y="15.24" length="middle" rot="R180"/>
<pin name="IO6" x="27.94" y="12.7" length="middle" rot="R180"/>
<pin name="IO5" x="27.94" y="10.16" length="middle" rot="R180"/>
<pin name="IO4" x="27.94" y="7.62" length="middle" rot="R180"/>
<pin name="IO3" x="27.94" y="5.08" length="middle" rot="R180"/>
<text x="0" y="48.26" size="1.778" layer="95">&gt;NAME</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="MC6850-ACIA" prefix="IC" uservalue="yes">
<description>&lt;b&gt;Dual In Line&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="DIL24" x="0" y="0"/>
</gates>
<devices>
<device name="-3" package="DIL24-3">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="10" pad="10"/>
<connect gate="G$1" pin="11" pad="11"/>
<connect gate="G$1" pin="12" pad="12"/>
<connect gate="G$1" pin="13" pad="13"/>
<connect gate="G$1" pin="14" pad="14"/>
<connect gate="G$1" pin="15" pad="15"/>
<connect gate="G$1" pin="16" pad="16"/>
<connect gate="G$1" pin="17" pad="17"/>
<connect gate="G$1" pin="18" pad="18"/>
<connect gate="G$1" pin="19" pad="19"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="20" pad="20"/>
<connect gate="G$1" pin="21" pad="21"/>
<connect gate="G$1" pin="22" pad="22"/>
<connect gate="G$1" pin="23" pad="23"/>
<connect gate="G$1" pin="24" pad="24"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
<connect gate="G$1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="-4" package="DIL24-4">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="10" pad="10"/>
<connect gate="G$1" pin="11" pad="11"/>
<connect gate="G$1" pin="12" pad="12"/>
<connect gate="G$1" pin="13" pad="13"/>
<connect gate="G$1" pin="14" pad="14"/>
<connect gate="G$1" pin="15" pad="15"/>
<connect gate="G$1" pin="16" pad="16"/>
<connect gate="G$1" pin="17" pad="17"/>
<connect gate="G$1" pin="18" pad="18"/>
<connect gate="G$1" pin="19" pad="19"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="20" pad="20"/>
<connect gate="G$1" pin="21" pad="21"/>
<connect gate="G$1" pin="22" pad="22"/>
<connect gate="G$1" pin="23" pad="23"/>
<connect gate="G$1" pin="24" pad="24"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
<connect gate="G$1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="-6" package="DIL24-6">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="10" pad="10"/>
<connect gate="G$1" pin="11" pad="11"/>
<connect gate="G$1" pin="12" pad="12"/>
<connect gate="G$1" pin="13" pad="13"/>
<connect gate="G$1" pin="14" pad="14"/>
<connect gate="G$1" pin="15" pad="15"/>
<connect gate="G$1" pin="16" pad="16"/>
<connect gate="G$1" pin="17" pad="17"/>
<connect gate="G$1" pin="18" pad="18"/>
<connect gate="G$1" pin="19" pad="19"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="20" pad="20"/>
<connect gate="G$1" pin="21" pad="21"/>
<connect gate="G$1" pin="22" pad="22"/>
<connect gate="G$1" pin="23" pad="23"/>
<connect gate="G$1" pin="24" pad="24"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
<connect gate="G$1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="-9" package="DIL24-9">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="10" pad="10"/>
<connect gate="G$1" pin="11" pad="11"/>
<connect gate="G$1" pin="12" pad="12"/>
<connect gate="G$1" pin="13" pad="13"/>
<connect gate="G$1" pin="14" pad="14"/>
<connect gate="G$1" pin="15" pad="15"/>
<connect gate="G$1" pin="16" pad="16"/>
<connect gate="G$1" pin="17" pad="17"/>
<connect gate="G$1" pin="18" pad="18"/>
<connect gate="G$1" pin="19" pad="19"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="20" pad="20"/>
<connect gate="G$1" pin="21" pad="21"/>
<connect gate="G$1" pin="22" pad="22"/>
<connect gate="G$1" pin="23" pad="23"/>
<connect gate="G$1" pin="24" pad="24"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
<connect gate="G$1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="AT28C256">
<gates>
<gate name="G$1" symbol="AT28C256" x="5.08" y="0"/>
</gates>
<devices>
<device name="" package="DIL28-4">
<connects>
<connect gate="G$1" pin="/CE" pad="1"/>
<connect gate="G$1" pin="/OE" pad="2"/>
<connect gate="G$1" pin="/WE" pad="3"/>
<connect gate="G$1" pin="A0" pad="4"/>
<connect gate="G$1" pin="A1" pad="5"/>
<connect gate="G$1" pin="A10" pad="14"/>
<connect gate="G$1" pin="A11" pad="15"/>
<connect gate="G$1" pin="A12" pad="16"/>
<connect gate="G$1" pin="A13" pad="17"/>
<connect gate="G$1" pin="A14" pad="18"/>
<connect gate="G$1" pin="A2" pad="6"/>
<connect gate="G$1" pin="A3" pad="7"/>
<connect gate="G$1" pin="A4" pad="8"/>
<connect gate="G$1" pin="A5" pad="9"/>
<connect gate="G$1" pin="A6" pad="10"/>
<connect gate="G$1" pin="A7" pad="11"/>
<connect gate="G$1" pin="A8" pad="12"/>
<connect gate="G$1" pin="A9" pad="13"/>
<connect gate="G$1" pin="GND" pad="19"/>
<connect gate="G$1" pin="IO0" pad="20"/>
<connect gate="G$1" pin="IO1" pad="21"/>
<connect gate="G$1" pin="IO2" pad="22"/>
<connect gate="G$1" pin="IO3" pad="23"/>
<connect gate="G$1" pin="IO4" pad="24"/>
<connect gate="G$1" pin="IO5" pad="25"/>
<connect gate="G$1" pin="IO6" pad="26"/>
<connect gate="G$1" pin="IO7" pad="27"/>
<connect gate="G$1" pin="VCC" pad="28"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="IC1" library="zilog" deviceset="Z80CPU" device="" value="Z80"/>
<part name="IC3" library="skeezix" deviceset="MC6850-ACIA" device="-6" value="MC6850"/>
<part name="AT28C256-ROM" library="skeezix" deviceset="AT28C256" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="IC1" gate="1" x="40.64" y="64.77"/>
<instance part="IC3" gate="G$1" x="99.06" y="66.04"/>
<instance part="AT28C256-ROM" gate="G$1" x="124.46" y="73.66"/>
</instances>
<busses>
</busses>
<nets>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
