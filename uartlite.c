<!doctype html public "-//W3C//DTD HTML 3.2//EN">
<html>
<head>
<title>/linux/drivers/serial/uartlite.c</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<base href="http://www.cs.fsu.edu/~baker/devices/lxr/http/">
<link rel="stylesheet" type="text/css" href="http://www.cs.fsu.edu/~baker/style.css" title="StyleSheet">
</head>
<body bgcolor=white>

<table width="100%"><tr>
<td bgcolor="660000" align=right>
<a href="../../index.html"><u><font color="cccc99" size=2>
Linux kernel &amp; device driver programming</font></u></a>
</td></tr></table>

<h2 align=center> Cross-Referenced
<A HREF="http://kernel.org">Linux</A> and <A HREF="http://examples.oreilly.com/linuxdrive3/">Device Driver</A> Code</h2>

<table width="100%"><tr><td bgcolor="cccc99" align="center">
   <b> [</b> <b><i>source navigation</i></b> <b>] </b> <b> [</b> <a href="diff/linux/drivers/serial/uartlite.c">diff markup</a> <b>] </b> <b> [</b> <a href="ident">identifier search</a> <b>] </b> <b> [</b> <a href="search">freetext search</a> <b>] </b> <b> [</b> <a href="find">file search</a> <b>] </b>
</td></tr></table>

<div align=center>
  
  <b>Version:</b>  <b>[</b>&nbsp;<a href="source/linux/drivers/serial/uartlite.c?v=2.6.11.8">2.6.11.8</a>&nbsp;<b>]</b> <b>[</b>&nbsp;<a href="source/linux/drivers/serial/uartlite.c?v=2.6.25">2.6.25</a>&nbsp;<b>]</b> <b>[</b>&nbsp;<a href="source/linux/drivers/serial/uartlite.c?v=2.6.25.8">2.6.25.8</a>&nbsp;<b>]</b> <b>[</b>&nbsp;<b><i>2.6.31.13</i></b>&nbsp;<b>]</b>
  <b>Architecture:</b>  <b>[</b>&nbsp;<b><i>i386</i></b>&nbsp;<b>]</b>
</div>

<pre>  <a name=L1 href="source/linux/drivers/serial/uartlite.c#L1">1</a> <b><i>/*</i></b>
  <a name=L2 href="source/linux/drivers/serial/uartlite.c#L2">2</a> <b><i> * uartlite.c: Serial driver for Xilinx uartlite serial controller</i></b>
  <a name=L3 href="source/linux/drivers/serial/uartlite.c#L3">3</a> <b><i> *</i></b>
  <a name=L4 href="source/linux/drivers/serial/uartlite.c#L4">4</a> <b><i> * Copyright (C) 2006 Peter Korsgaard &lt;jacmet@sunsite.dk&gt;</i></b>
  <a name=L5 href="source/linux/drivers/serial/uartlite.c#L5">5</a> <b><i> * Copyright (C) 2007 Secret Lab Technologies Ltd.</i></b>
  <a name=L6 href="source/linux/drivers/serial/uartlite.c#L6">6</a> <b><i> *</i></b>
  <a name=L7 href="source/linux/drivers/serial/uartlite.c#L7">7</a> <b><i> * This file is licensed under the terms of the GNU General Public License</i></b>
  <a name=L8 href="source/linux/drivers/serial/uartlite.c#L8">8</a> <b><i> * version 2.  This program is licensed "as is" without any warranty of any</i></b>
  <a name=L9 href="source/linux/drivers/serial/uartlite.c#L9">9</a> <b><i> * kind, whether express or implied.</i></b>
 <a name=L10 href="source/linux/drivers/serial/uartlite.c#L10">10</a> <b><i> */</i></b>
 <a name=L11 href="source/linux/drivers/serial/uartlite.c#L11">11</a> 
 <a name=L12 href="source/linux/drivers/serial/uartlite.c#L12">12</a> #include &lt;linux/platform_device.h&gt;
 <a name=L13 href="source/linux/drivers/serial/uartlite.c#L13">13</a> #include &lt;linux/module.h&gt;
 <a name=L14 href="source/linux/drivers/serial/uartlite.c#L14">14</a> #include &lt;linux/console.h&gt;
 <a name=L15 href="source/linux/drivers/serial/uartlite.c#L15">15</a> #include &lt;linux/serial.h&gt;
 <a name=L16 href="source/linux/drivers/serial/uartlite.c#L16">16</a> #include &lt;linux/serial_core.h&gt;
 <a name=L17 href="source/linux/drivers/serial/uartlite.c#L17">17</a> #include &lt;linux/tty.h&gt;
 <a name=L18 href="source/linux/drivers/serial/uartlite.c#L18">18</a> #include &lt;linux/delay.h&gt;
 <a name=L19 href="source/linux/drivers/serial/uartlite.c#L19">19</a> #include &lt;linux/interrupt.h&gt;
 <a name=L20 href="source/linux/drivers/serial/uartlite.c#L20">20</a> #include &lt;linux/init.h&gt;
 <a name=L21 href="source/linux/drivers/serial/uartlite.c#L21">21</a> #include &lt;asm/io.h&gt;
 <a name=L22 href="source/linux/drivers/serial/uartlite.c#L22">22</a> #if defined(CONFIG_OF)
 <a name=L23 href="source/linux/drivers/serial/uartlite.c#L23">23</a> #include &lt;linux/of.h&gt;
 <a name=L24 href="source/linux/drivers/serial/uartlite.c#L24">24</a> #include &lt;linux/of_device.h&gt;
 <a name=L25 href="source/linux/drivers/serial/uartlite.c#L25">25</a> #include &lt;linux/of_platform.h&gt;
 <a name=L26 href="source/linux/drivers/serial/uartlite.c#L26">26</a> 
 <a name=L27 href="source/linux/drivers/serial/uartlite.c#L27">27</a> <b><i>/* Match table for of_platform binding */</i></b>
 <a name=L28 href="source/linux/drivers/serial/uartlite.c#L28">28</a> static struct <a href="ident?i=of_device_id">of_device_id</a> ulite_of_match[] <a href="ident?i=__devinitdata">__devinitdata</a> = {
 <a name=L29 href="source/linux/drivers/serial/uartlite.c#L29">29</a>         { .compatible = <i>"xlnx,opb-uartlite-1.00.b"</i>, },
 <a name=L30 href="source/linux/drivers/serial/uartlite.c#L30">30</a>         { .compatible = <i>"xlnx,xps-uartlite-1.00.a"</i>, },
 <a name=L31 href="source/linux/drivers/serial/uartlite.c#L31">31</a>         {}
 <a name=L32 href="source/linux/drivers/serial/uartlite.c#L32">32</a> };
 <a name=L33 href="source/linux/drivers/serial/uartlite.c#L33">33</a> <a href="ident?i=MODULE_DEVICE_TABLE">MODULE_DEVICE_TABLE</a>(<a href="ident?i=of">of</a>, ulite_of_match);
 <a name=L34 href="source/linux/drivers/serial/uartlite.c#L34">34</a> 
 <a name=L35 href="source/linux/drivers/serial/uartlite.c#L35">35</a> #endif
 <a name=L36 href="source/linux/drivers/serial/uartlite.c#L36">36</a> 
 <a name=L37 href="source/linux/drivers/serial/uartlite.c#L37">37</a> #define <a href="ident?i=ULITE_NAME">ULITE_NAME</a>              <i>"ttyUL"</i>
 <a name=L38 href="source/linux/drivers/serial/uartlite.c#L38">38</a> #define <a href="ident?i=ULITE_MAJOR">ULITE_MAJOR</a>             204
 <a name=L39 href="source/linux/drivers/serial/uartlite.c#L39">39</a> #define <a href="ident?i=ULITE_MINOR">ULITE_MINOR</a>             187
 <a name=L40 href="source/linux/drivers/serial/uartlite.c#L40">40</a> #define <a href="ident?i=ULITE_NR_UARTS">ULITE_NR_UARTS</a>          4
 <a name=L41 href="source/linux/drivers/serial/uartlite.c#L41">41</a> 
 <a name=L42 href="source/linux/drivers/serial/uartlite.c#L42">42</a> <b><i>/* ---------------------------------------------------------------------</i></b>
 <a name=L43 href="source/linux/drivers/serial/uartlite.c#L43">43</a> <b><i> * Register definitions</i></b>
 <a name=L44 href="source/linux/drivers/serial/uartlite.c#L44">44</a> <b><i> *</i></b>
 <a name=L45 href="source/linux/drivers/serial/uartlite.c#L45">45</a> <b><i> * For register details see datasheet:</i></b>
 <a name=L46 href="source/linux/drivers/serial/uartlite.c#L46">46</a> <b><i> * <a href="http://www.xilinx.com/bvdocs/ipcenter/data_sheet/opb_uartlite.pdf">http://www.xilinx.com/bvdocs/ipcenter/data_sheet/opb_uartlite.pdf</a></i></b>
 <a name=L47 href="source/linux/drivers/serial/uartlite.c#L47">47</a> <b><i> */</i></b>
 <a name=L48 href="source/linux/drivers/serial/uartlite.c#L48">48</a> 
 <a name=L49 href="source/linux/drivers/serial/uartlite.c#L49">49</a> #define <a href="ident?i=ULITE_RX">ULITE_RX</a>                0x00
 <a name=L50 href="source/linux/drivers/serial/uartlite.c#L50">50</a> #define <a href="ident?i=ULITE_TX">ULITE_TX</a>                0x04
 <a name=L51 href="source/linux/drivers/serial/uartlite.c#L51">51</a> #define <a href="ident?i=ULITE_STATUS">ULITE_STATUS</a>            0x08
 <a name=L52 href="source/linux/drivers/serial/uartlite.c#L52">52</a> #define <a href="ident?i=ULITE_CONTROL">ULITE_CONTROL</a>           0x0c
 <a name=L53 href="source/linux/drivers/serial/uartlite.c#L53">53</a> 
 <a name=L54 href="source/linux/drivers/serial/uartlite.c#L54">54</a> #define <a href="ident?i=ULITE_REGION">ULITE_REGION</a>            16
 <a name=L55 href="source/linux/drivers/serial/uartlite.c#L55">55</a> 
 <a name=L56 href="source/linux/drivers/serial/uartlite.c#L56">56</a> #define <a href="ident?i=ULITE_STATUS_RXVALID">ULITE_STATUS_RXVALID</a>    0x01
 <a name=L57 href="source/linux/drivers/serial/uartlite.c#L57">57</a> #define <a href="ident?i=ULITE_STATUS_RXFULL">ULITE_STATUS_RXFULL</a>     0x02
 <a name=L58 href="source/linux/drivers/serial/uartlite.c#L58">58</a> #define <a href="ident?i=ULITE_STATUS_TXEMPTY">ULITE_STATUS_TXEMPTY</a>    0x04
 <a name=L59 href="source/linux/drivers/serial/uartlite.c#L59">59</a> #define <a href="ident?i=ULITE_STATUS_TXFULL">ULITE_STATUS_TXFULL</a>     0x08
 <a name=L60 href="source/linux/drivers/serial/uartlite.c#L60">60</a> #define <a href="ident?i=ULITE_STATUS_IE">ULITE_STATUS_IE</a>         0x10
 <a name=L61 href="source/linux/drivers/serial/uartlite.c#L61">61</a> #define <a href="ident?i=ULITE_STATUS_OVERRUN">ULITE_STATUS_OVERRUN</a>    0x20
 <a name=L62 href="source/linux/drivers/serial/uartlite.c#L62">62</a> #define <a href="ident?i=ULITE_STATUS_FRAME">ULITE_STATUS_FRAME</a>      0x40
 <a name=L63 href="source/linux/drivers/serial/uartlite.c#L63">63</a> #define <a href="ident?i=ULITE_STATUS_PARITY">ULITE_STATUS_PARITY</a>     0x80
 <a name=L64 href="source/linux/drivers/serial/uartlite.c#L64">64</a> 
 <a name=L65 href="source/linux/drivers/serial/uartlite.c#L65">65</a> #define <a href="ident?i=ULITE_CONTROL_RST_TX">ULITE_CONTROL_RST_TX</a>    0x01
 <a name=L66 href="source/linux/drivers/serial/uartlite.c#L66">66</a> #define <a href="ident?i=ULITE_CONTROL_RST_RX">ULITE_CONTROL_RST_RX</a>    0x02
 <a name=L67 href="source/linux/drivers/serial/uartlite.c#L67">67</a> #define <a href="ident?i=ULITE_CONTROL_IE">ULITE_CONTROL_IE</a>        0x10
 <a name=L68 href="source/linux/drivers/serial/uartlite.c#L68">68</a> 
 <a name=L69 href="source/linux/drivers/serial/uartlite.c#L69">69</a> 
 <a name=L70 href="source/linux/drivers/serial/uartlite.c#L70">70</a> static struct <a href="ident?i=uart_port">uart_port</a> <a href="ident?i=ulite_ports">ulite_ports</a>[<a href="ident?i=ULITE_NR_UARTS">ULITE_NR_UARTS</a>];
 <a name=L71 href="source/linux/drivers/serial/uartlite.c#L71">71</a> 
 <a name=L72 href="source/linux/drivers/serial/uartlite.c#L72">72</a> <b><i>/* ---------------------------------------------------------------------</i></b>
 <a name=L73 href="source/linux/drivers/serial/uartlite.c#L73">73</a> <b><i> * Core UART driver operations</i></b>
 <a name=L74 href="source/linux/drivers/serial/uartlite.c#L74">74</a> <b><i> */</i></b>
 <a name=L75 href="source/linux/drivers/serial/uartlite.c#L75">75</a> 
 <a name=L76 href="source/linux/drivers/serial/uartlite.c#L76">76</a> static int <a href="ident?i=ulite_receive">ulite_receive</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>, int <a href="ident?i=stat">stat</a>)
 <a name=L77 href="source/linux/drivers/serial/uartlite.c#L77">77</a> {
 <a name=L78 href="source/linux/drivers/serial/uartlite.c#L78">78</a>         struct <a href="ident?i=tty_struct">tty_struct</a> *<a href="ident?i=tty">tty</a> = <a href="ident?i=port">port</a>-&gt;<a href="ident?i=info">info</a>-&gt;<a href="ident?i=port">port</a>.<a href="ident?i=tty">tty</a>;
 <a name=L79 href="source/linux/drivers/serial/uartlite.c#L79">79</a>         unsigned char <a href="ident?i=ch">ch</a> = 0;
 <a name=L80 href="source/linux/drivers/serial/uartlite.c#L80">80</a>         char <a href="ident?i=flag">flag</a> = <a href="ident?i=TTY_NORMAL">TTY_NORMAL</a>;
 <a name=L81 href="source/linux/drivers/serial/uartlite.c#L81">81</a> 
 <a name=L82 href="source/linux/drivers/serial/uartlite.c#L82">82</a>         if ((<a href="ident?i=stat">stat</a> &amp; (<a href="ident?i=ULITE_STATUS_RXVALID">ULITE_STATUS_RXVALID</a> | <a href="ident?i=ULITE_STATUS_OVERRUN">ULITE_STATUS_OVERRUN</a>
 <a name=L83 href="source/linux/drivers/serial/uartlite.c#L83">83</a>                      | <a href="ident?i=ULITE_STATUS_FRAME">ULITE_STATUS_FRAME</a>)) == 0)
 <a name=L84 href="source/linux/drivers/serial/uartlite.c#L84">84</a>                 return 0;
 <a name=L85 href="source/linux/drivers/serial/uartlite.c#L85">85</a> 
 <a name=L86 href="source/linux/drivers/serial/uartlite.c#L86">86</a>         <b><i>/* stats */</i></b>
 <a name=L87 href="source/linux/drivers/serial/uartlite.c#L87">87</a>         if (<a href="ident?i=stat">stat</a> &amp; <a href="ident?i=ULITE_STATUS_RXVALID">ULITE_STATUS_RXVALID</a>) {
 <a name=L88 href="source/linux/drivers/serial/uartlite.c#L88">88</a>                 <a href="ident?i=port">port</a>-&gt;<a href="ident?i=icount">icount</a>.<a href="ident?i=rx">rx</a>++;
 <a name=L89 href="source/linux/drivers/serial/uartlite.c#L89">89</a>                 <a href="ident?i=ch">ch</a> = <a href="ident?i=readb">readb</a>(<a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a> + <a href="ident?i=ULITE_RX">ULITE_RX</a>);
 <a name=L90 href="source/linux/drivers/serial/uartlite.c#L90">90</a> 
 <a name=L91 href="source/linux/drivers/serial/uartlite.c#L91">91</a>                 if (<a href="ident?i=stat">stat</a> &amp; <a href="ident?i=ULITE_STATUS_PARITY">ULITE_STATUS_PARITY</a>)
 <a name=L92 href="source/linux/drivers/serial/uartlite.c#L92">92</a>                         <a href="ident?i=port">port</a>-&gt;<a href="ident?i=icount">icount</a>.<a href="ident?i=parity">parity</a>++;
 <a name=L93 href="source/linux/drivers/serial/uartlite.c#L93">93</a>         }
 <a name=L94 href="source/linux/drivers/serial/uartlite.c#L94">94</a> 
 <a name=L95 href="source/linux/drivers/serial/uartlite.c#L95">95</a>         if (<a href="ident?i=stat">stat</a> &amp; <a href="ident?i=ULITE_STATUS_OVERRUN">ULITE_STATUS_OVERRUN</a>)
 <a name=L96 href="source/linux/drivers/serial/uartlite.c#L96">96</a>                 <a href="ident?i=port">port</a>-&gt;<a href="ident?i=icount">icount</a>.overrun++;
 <a name=L97 href="source/linux/drivers/serial/uartlite.c#L97">97</a> 
 <a name=L98 href="source/linux/drivers/serial/uartlite.c#L98">98</a>         if (<a href="ident?i=stat">stat</a> &amp; <a href="ident?i=ULITE_STATUS_FRAME">ULITE_STATUS_FRAME</a>)
 <a name=L99 href="source/linux/drivers/serial/uartlite.c#L99">99</a>                 <a href="ident?i=port">port</a>-&gt;<a href="ident?i=icount">icount</a>.<a href="ident?i=frame">frame</a>++;
<a name=L100 href="source/linux/drivers/serial/uartlite.c#L100">100</a> 
<a name=L101 href="source/linux/drivers/serial/uartlite.c#L101">101</a> 
<a name=L102 href="source/linux/drivers/serial/uartlite.c#L102">102</a>         <b><i>/* drop byte with parity error if IGNPAR specificed */</i></b>
<a name=L103 href="source/linux/drivers/serial/uartlite.c#L103">103</a>         if (<a href="ident?i=stat">stat</a> &amp; <a href="ident?i=port">port</a>-&gt;ignore_status_mask &amp; <a href="ident?i=ULITE_STATUS_PARITY">ULITE_STATUS_PARITY</a>)
<a name=L104 href="source/linux/drivers/serial/uartlite.c#L104">104</a>                 <a href="ident?i=stat">stat</a> &amp;= ~ULITE_STATUS_RXVALID;
<a name=L105 href="source/linux/drivers/serial/uartlite.c#L105">105</a> 
<a name=L106 href="source/linux/drivers/serial/uartlite.c#L106">106</a>         <a href="ident?i=stat">stat</a> &amp;= <a href="ident?i=port">port</a>-&gt;read_status_mask;
<a name=L107 href="source/linux/drivers/serial/uartlite.c#L107">107</a> 
<a name=L108 href="source/linux/drivers/serial/uartlite.c#L108">108</a>         if (<a href="ident?i=stat">stat</a> &amp; <a href="ident?i=ULITE_STATUS_PARITY">ULITE_STATUS_PARITY</a>)
<a name=L109 href="source/linux/drivers/serial/uartlite.c#L109">109</a>                 <a href="ident?i=flag">flag</a> = <a href="ident?i=TTY_PARITY">TTY_PARITY</a>;
<a name=L110 href="source/linux/drivers/serial/uartlite.c#L110">110</a> 
<a name=L111 href="source/linux/drivers/serial/uartlite.c#L111">111</a> 
<a name=L112 href="source/linux/drivers/serial/uartlite.c#L112">112</a>         <a href="ident?i=stat">stat</a> &amp;= ~port-&gt;ignore_status_mask;
<a name=L113 href="source/linux/drivers/serial/uartlite.c#L113">113</a> 
<a name=L114 href="source/linux/drivers/serial/uartlite.c#L114">114</a>         if (<a href="ident?i=stat">stat</a> &amp; <a href="ident?i=ULITE_STATUS_RXVALID">ULITE_STATUS_RXVALID</a>)
<a name=L115 href="source/linux/drivers/serial/uartlite.c#L115">115</a>                 <a href="ident?i=tty_insert_flip_char">tty_insert_flip_char</a>(<a href="ident?i=tty">tty</a>, <a href="ident?i=ch">ch</a>, <a href="ident?i=flag">flag</a>);
<a name=L116 href="source/linux/drivers/serial/uartlite.c#L116">116</a> 
<a name=L117 href="source/linux/drivers/serial/uartlite.c#L117">117</a>         if (<a href="ident?i=stat">stat</a> &amp; <a href="ident?i=ULITE_STATUS_FRAME">ULITE_STATUS_FRAME</a>)
<a name=L118 href="source/linux/drivers/serial/uartlite.c#L118">118</a>                 <a href="ident?i=tty_insert_flip_char">tty_insert_flip_char</a>(<a href="ident?i=tty">tty</a>, 0, <a href="ident?i=TTY_FRAME">TTY_FRAME</a>);
<a name=L119 href="source/linux/drivers/serial/uartlite.c#L119">119</a> 
<a name=L120 href="source/linux/drivers/serial/uartlite.c#L120">120</a>         if (<a href="ident?i=stat">stat</a> &amp; <a href="ident?i=ULITE_STATUS_OVERRUN">ULITE_STATUS_OVERRUN</a>)
<a name=L121 href="source/linux/drivers/serial/uartlite.c#L121">121</a>                 <a href="ident?i=tty_insert_flip_char">tty_insert_flip_char</a>(<a href="ident?i=tty">tty</a>, 0, <a href="ident?i=TTY_OVERRUN">TTY_OVERRUN</a>);
<a name=L122 href="source/linux/drivers/serial/uartlite.c#L122">122</a> 
<a name=L123 href="source/linux/drivers/serial/uartlite.c#L123">123</a>         return 1;
<a name=L124 href="source/linux/drivers/serial/uartlite.c#L124">124</a> }
<a name=L125 href="source/linux/drivers/serial/uartlite.c#L125">125</a> 
<a name=L126 href="source/linux/drivers/serial/uartlite.c#L126">126</a> static int <a href="ident?i=ulite_transmit">ulite_transmit</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>, int <a href="ident?i=stat">stat</a>)
<a name=L127 href="source/linux/drivers/serial/uartlite.c#L127">127</a> {
<a name=L128 href="source/linux/drivers/serial/uartlite.c#L128">128</a>         struct <a href="ident?i=circ_buf">circ_buf</a> *xmit  = &amp;<a href="ident?i=port">port</a>-&gt;<a href="ident?i=info">info</a>-&gt;xmit;
<a name=L129 href="source/linux/drivers/serial/uartlite.c#L129">129</a> 
<a name=L130 href="source/linux/drivers/serial/uartlite.c#L130">130</a>         if (<a href="ident?i=stat">stat</a> &amp; <a href="ident?i=ULITE_STATUS_TXFULL">ULITE_STATUS_TXFULL</a>)
<a name=L131 href="source/linux/drivers/serial/uartlite.c#L131">131</a>                 return 0;
<a name=L132 href="source/linux/drivers/serial/uartlite.c#L132">132</a> 
<a name=L133 href="source/linux/drivers/serial/uartlite.c#L133">133</a>         if (<a href="ident?i=port">port</a>-&gt;x_char) {
<a name=L134 href="source/linux/drivers/serial/uartlite.c#L134">134</a>                 <a href="ident?i=writeb">writeb</a>(<a href="ident?i=port">port</a>-&gt;x_char, <a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a> + <a href="ident?i=ULITE_TX">ULITE_TX</a>);
<a name=L135 href="source/linux/drivers/serial/uartlite.c#L135">135</a>                 <a href="ident?i=port">port</a>-&gt;x_char = 0;
<a name=L136 href="source/linux/drivers/serial/uartlite.c#L136">136</a>                 <a href="ident?i=port">port</a>-&gt;<a href="ident?i=icount">icount</a>.tx++;
<a name=L137 href="source/linux/drivers/serial/uartlite.c#L137">137</a>                 return 1;
<a name=L138 href="source/linux/drivers/serial/uartlite.c#L138">138</a>         }
<a name=L139 href="source/linux/drivers/serial/uartlite.c#L139">139</a> 
<a name=L140 href="source/linux/drivers/serial/uartlite.c#L140">140</a>         if (<a href="ident?i=uart_circ_empty">uart_circ_empty</a>(xmit) || <a href="ident?i=uart_tx_stopped">uart_tx_stopped</a>(<a href="ident?i=port">port</a>))
<a name=L141 href="source/linux/drivers/serial/uartlite.c#L141">141</a>                 return 0;
<a name=L142 href="source/linux/drivers/serial/uartlite.c#L142">142</a> 
<a name=L143 href="source/linux/drivers/serial/uartlite.c#L143">143</a>         <a href="ident?i=writeb">writeb</a>(xmit-&gt;<a href="ident?i=buf">buf</a>[xmit-&gt;tail], <a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a> + <a href="ident?i=ULITE_TX">ULITE_TX</a>);
<a name=L144 href="source/linux/drivers/serial/uartlite.c#L144">144</a>         xmit-&gt;tail = (xmit-&gt;tail + 1) &amp; (<a href="ident?i=UART_XMIT_SIZE">UART_XMIT_SIZE</a>-1);
<a name=L145 href="source/linux/drivers/serial/uartlite.c#L145">145</a>         <a href="ident?i=port">port</a>-&gt;<a href="ident?i=icount">icount</a>.tx++;
<a name=L146 href="source/linux/drivers/serial/uartlite.c#L146">146</a> 
<a name=L147 href="source/linux/drivers/serial/uartlite.c#L147">147</a>         <b><i>/* wake up */</i></b>
<a name=L148 href="source/linux/drivers/serial/uartlite.c#L148">148</a>         if (<a href="ident?i=uart_circ_chars_pending">uart_circ_chars_pending</a>(xmit) &lt; <a href="ident?i=WAKEUP_CHARS">WAKEUP_CHARS</a>)
<a name=L149 href="source/linux/drivers/serial/uartlite.c#L149">149</a>                 <a href="ident?i=uart_write_wakeup">uart_write_wakeup</a>(<a href="ident?i=port">port</a>);
<a name=L150 href="source/linux/drivers/serial/uartlite.c#L150">150</a> 
<a name=L151 href="source/linux/drivers/serial/uartlite.c#L151">151</a>         return 1;
<a name=L152 href="source/linux/drivers/serial/uartlite.c#L152">152</a> }
<a name=L153 href="source/linux/drivers/serial/uartlite.c#L153">153</a> 
<a name=L154 href="source/linux/drivers/serial/uartlite.c#L154">154</a> static <a href="ident?i=irqreturn_t">irqreturn_t</a> <a href="ident?i=ulite_isr">ulite_isr</a>(int <a href="ident?i=irq">irq</a>, void *<a href="ident?i=dev_id">dev_id</a>)
<a name=L155 href="source/linux/drivers/serial/uartlite.c#L155">155</a> {
<a name=L156 href="source/linux/drivers/serial/uartlite.c#L156">156</a>         struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a> = <a href="ident?i=dev_id">dev_id</a>;
<a name=L157 href="source/linux/drivers/serial/uartlite.c#L157">157</a>         int busy;
<a name=L158 href="source/linux/drivers/serial/uartlite.c#L158">158</a> 
<a name=L159 href="source/linux/drivers/serial/uartlite.c#L159">159</a>         do {
<a name=L160 href="source/linux/drivers/serial/uartlite.c#L160">160</a>                 int <a href="ident?i=stat">stat</a> = <a href="ident?i=readb">readb</a>(<a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a> + <a href="ident?i=ULITE_STATUS">ULITE_STATUS</a>);
<a name=L161 href="source/linux/drivers/serial/uartlite.c#L161">161</a>                 busy  = <a href="ident?i=ulite_receive">ulite_receive</a>(<a href="ident?i=port">port</a>, <a href="ident?i=stat">stat</a>);
<a name=L162 href="source/linux/drivers/serial/uartlite.c#L162">162</a>                 busy |= <a href="ident?i=ulite_transmit">ulite_transmit</a>(<a href="ident?i=port">port</a>, <a href="ident?i=stat">stat</a>);
<a name=L163 href="source/linux/drivers/serial/uartlite.c#L163">163</a>         } while (busy);
<a name=L164 href="source/linux/drivers/serial/uartlite.c#L164">164</a> 
<a name=L165 href="source/linux/drivers/serial/uartlite.c#L165">165</a>         <a href="ident?i=tty_flip_buffer_push">tty_flip_buffer_push</a>(<a href="ident?i=port">port</a>-&gt;<a href="ident?i=info">info</a>-&gt;<a href="ident?i=port">port</a>.<a href="ident?i=tty">tty</a>);
<a name=L166 href="source/linux/drivers/serial/uartlite.c#L166">166</a> 
<a name=L167 href="source/linux/drivers/serial/uartlite.c#L167">167</a>         return <a href="ident?i=IRQ_HANDLED">IRQ_HANDLED</a>;
<a name=L168 href="source/linux/drivers/serial/uartlite.c#L168">168</a> }
<a name=L169 href="source/linux/drivers/serial/uartlite.c#L169">169</a> 
<a name=L170 href="source/linux/drivers/serial/uartlite.c#L170">170</a> static unsigned int <a href="ident?i=ulite_tx_empty">ulite_tx_empty</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>)
<a name=L171 href="source/linux/drivers/serial/uartlite.c#L171">171</a> {
<a name=L172 href="source/linux/drivers/serial/uartlite.c#L172">172</a>         unsigned long <a href="ident?i=flags">flags</a>;
<a name=L173 href="source/linux/drivers/serial/uartlite.c#L173">173</a>         unsigned int <a href="ident?i=ret">ret</a>;
<a name=L174 href="source/linux/drivers/serial/uartlite.c#L174">174</a> 
<a name=L175 href="source/linux/drivers/serial/uartlite.c#L175">175</a>         <a href="ident?i=spin_lock_irqsave">spin_lock_irqsave</a>(&amp;<a href="ident?i=port">port</a>-&gt;<a href="ident?i=lock">lock</a>, <a href="ident?i=flags">flags</a>);
<a name=L176 href="source/linux/drivers/serial/uartlite.c#L176">176</a>         <a href="ident?i=ret">ret</a> = <a href="ident?i=readb">readb</a>(<a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a> + <a href="ident?i=ULITE_STATUS">ULITE_STATUS</a>);
<a name=L177 href="source/linux/drivers/serial/uartlite.c#L177">177</a>         <a href="ident?i=spin_unlock_irqrestore">spin_unlock_irqrestore</a>(&amp;<a href="ident?i=port">port</a>-&gt;<a href="ident?i=lock">lock</a>, <a href="ident?i=flags">flags</a>);
<a name=L178 href="source/linux/drivers/serial/uartlite.c#L178">178</a> 
<a name=L179 href="source/linux/drivers/serial/uartlite.c#L179">179</a>         return <a href="ident?i=ret">ret</a> &amp; <a href="ident?i=ULITE_STATUS_TXEMPTY">ULITE_STATUS_TXEMPTY</a> ? <a href="ident?i=TIOCSER_TEMT">TIOCSER_TEMT</a> : 0;
<a name=L180 href="source/linux/drivers/serial/uartlite.c#L180">180</a> }
<a name=L181 href="source/linux/drivers/serial/uartlite.c#L181">181</a> 
<a name=L182 href="source/linux/drivers/serial/uartlite.c#L182">182</a> static unsigned int <a href="ident?i=ulite_get_mctrl">ulite_get_mctrl</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>)
<a name=L183 href="source/linux/drivers/serial/uartlite.c#L183">183</a> {
<a name=L184 href="source/linux/drivers/serial/uartlite.c#L184">184</a>         return <a href="ident?i=TIOCM_CTS">TIOCM_CTS</a> | <a href="ident?i=TIOCM_DSR">TIOCM_DSR</a> | <a href="ident?i=TIOCM_CAR">TIOCM_CAR</a>;
<a name=L185 href="source/linux/drivers/serial/uartlite.c#L185">185</a> }
<a name=L186 href="source/linux/drivers/serial/uartlite.c#L186">186</a> 
<a name=L187 href="source/linux/drivers/serial/uartlite.c#L187">187</a> static void <a href="ident?i=ulite_set_mctrl">ulite_set_mctrl</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>, unsigned int mctrl)
<a name=L188 href="source/linux/drivers/serial/uartlite.c#L188">188</a> {
<a name=L189 href="source/linux/drivers/serial/uartlite.c#L189">189</a>         <b><i>/* N/A */</i></b>
<a name=L190 href="source/linux/drivers/serial/uartlite.c#L190">190</a> }
<a name=L191 href="source/linux/drivers/serial/uartlite.c#L191">191</a> 
<a name=L192 href="source/linux/drivers/serial/uartlite.c#L192">192</a> static void <a href="ident?i=ulite_stop_tx">ulite_stop_tx</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>)
<a name=L193 href="source/linux/drivers/serial/uartlite.c#L193">193</a> {
<a name=L194 href="source/linux/drivers/serial/uartlite.c#L194">194</a>         <b><i>/* N/A */</i></b>
<a name=L195 href="source/linux/drivers/serial/uartlite.c#L195">195</a> }
<a name=L196 href="source/linux/drivers/serial/uartlite.c#L196">196</a> 
<a name=L197 href="source/linux/drivers/serial/uartlite.c#L197">197</a> static void <a href="ident?i=ulite_start_tx">ulite_start_tx</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>)
<a name=L198 href="source/linux/drivers/serial/uartlite.c#L198">198</a> {
<a name=L199 href="source/linux/drivers/serial/uartlite.c#L199">199</a>         <a href="ident?i=ulite_transmit">ulite_transmit</a>(<a href="ident?i=port">port</a>, <a href="ident?i=readb">readb</a>(<a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a> + <a href="ident?i=ULITE_STATUS">ULITE_STATUS</a>));
<a name=L200 href="source/linux/drivers/serial/uartlite.c#L200">200</a> }
<a name=L201 href="source/linux/drivers/serial/uartlite.c#L201">201</a> 
<a name=L202 href="source/linux/drivers/serial/uartlite.c#L202">202</a> static void <a href="ident?i=ulite_stop_rx">ulite_stop_rx</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>)
<a name=L203 href="source/linux/drivers/serial/uartlite.c#L203">203</a> {
<a name=L204 href="source/linux/drivers/serial/uartlite.c#L204">204</a>         <b><i>/* don't forward any more data (like !CREAD) */</i></b>
<a name=L205 href="source/linux/drivers/serial/uartlite.c#L205">205</a>         <a href="ident?i=port">port</a>-&gt;ignore_status_mask = <a href="ident?i=ULITE_STATUS_RXVALID">ULITE_STATUS_RXVALID</a> | <a href="ident?i=ULITE_STATUS_PARITY">ULITE_STATUS_PARITY</a>
<a name=L206 href="source/linux/drivers/serial/uartlite.c#L206">206</a>                 | <a href="ident?i=ULITE_STATUS_FRAME">ULITE_STATUS_FRAME</a> | <a href="ident?i=ULITE_STATUS_OVERRUN">ULITE_STATUS_OVERRUN</a>;
<a name=L207 href="source/linux/drivers/serial/uartlite.c#L207">207</a> }
<a name=L208 href="source/linux/drivers/serial/uartlite.c#L208">208</a> 
<a name=L209 href="source/linux/drivers/serial/uartlite.c#L209">209</a> static void <a href="ident?i=ulite_enable_ms">ulite_enable_ms</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>)
<a name=L210 href="source/linux/drivers/serial/uartlite.c#L210">210</a> {
<a name=L211 href="source/linux/drivers/serial/uartlite.c#L211">211</a>         <b><i>/* N/A */</i></b>
<a name=L212 href="source/linux/drivers/serial/uartlite.c#L212">212</a> }
<a name=L213 href="source/linux/drivers/serial/uartlite.c#L213">213</a> 
<a name=L214 href="source/linux/drivers/serial/uartlite.c#L214">214</a> static void <a href="ident?i=ulite_break_ctl">ulite_break_ctl</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>, int <a href="ident?i=ctl">ctl</a>)
<a name=L215 href="source/linux/drivers/serial/uartlite.c#L215">215</a> {
<a name=L216 href="source/linux/drivers/serial/uartlite.c#L216">216</a>         <b><i>/* N/A */</i></b>
<a name=L217 href="source/linux/drivers/serial/uartlite.c#L217">217</a> }
<a name=L218 href="source/linux/drivers/serial/uartlite.c#L218">218</a> 
<a name=L219 href="source/linux/drivers/serial/uartlite.c#L219">219</a> static int <a href="ident?i=ulite_startup">ulite_startup</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>)
<a name=L220 href="source/linux/drivers/serial/uartlite.c#L220">220</a> {
<a name=L221 href="source/linux/drivers/serial/uartlite.c#L221">221</a>         int <a href="ident?i=ret">ret</a>;
<a name=L222 href="source/linux/drivers/serial/uartlite.c#L222">222</a> 
<a name=L223 href="source/linux/drivers/serial/uartlite.c#L223">223</a>         <a href="ident?i=ret">ret</a> = <a href="ident?i=request_irq">request_irq</a>(<a href="ident?i=port">port</a>-&gt;<a href="ident?i=irq">irq</a>, <a href="ident?i=ulite_isr">ulite_isr</a>,
<a name=L224 href="source/linux/drivers/serial/uartlite.c#L224">224</a>                           <a href="ident?i=IRQF_DISABLED">IRQF_DISABLED</a> | <a href="ident?i=IRQF_SAMPLE_RANDOM">IRQF_SAMPLE_RANDOM</a>, <i>"uartlite"</i>, <a href="ident?i=port">port</a>);
<a name=L225 href="source/linux/drivers/serial/uartlite.c#L225">225</a>         if (<a href="ident?i=ret">ret</a>)
<a name=L226 href="source/linux/drivers/serial/uartlite.c#L226">226</a>                 return <a href="ident?i=ret">ret</a>;
<a name=L227 href="source/linux/drivers/serial/uartlite.c#L227">227</a> 
<a name=L228 href="source/linux/drivers/serial/uartlite.c#L228">228</a>         <a href="ident?i=writeb">writeb</a>(<a href="ident?i=ULITE_CONTROL_RST_RX">ULITE_CONTROL_RST_RX</a> | <a href="ident?i=ULITE_CONTROL_RST_TX">ULITE_CONTROL_RST_TX</a>,
<a name=L229 href="source/linux/drivers/serial/uartlite.c#L229">229</a>                <a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a> + <a href="ident?i=ULITE_CONTROL">ULITE_CONTROL</a>);
<a name=L230 href="source/linux/drivers/serial/uartlite.c#L230">230</a>         <a href="ident?i=writeb">writeb</a>(<a href="ident?i=ULITE_CONTROL_IE">ULITE_CONTROL_IE</a>, <a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a> + <a href="ident?i=ULITE_CONTROL">ULITE_CONTROL</a>);
<a name=L231 href="source/linux/drivers/serial/uartlite.c#L231">231</a> 
<a name=L232 href="source/linux/drivers/serial/uartlite.c#L232">232</a>         return 0;
<a name=L233 href="source/linux/drivers/serial/uartlite.c#L233">233</a> }
<a name=L234 href="source/linux/drivers/serial/uartlite.c#L234">234</a> 
<a name=L235 href="source/linux/drivers/serial/uartlite.c#L235">235</a> static void <a href="ident?i=ulite_shutdown">ulite_shutdown</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>)
<a name=L236 href="source/linux/drivers/serial/uartlite.c#L236">236</a> {
<a name=L237 href="source/linux/drivers/serial/uartlite.c#L237">237</a>         <a href="ident?i=writeb">writeb</a>(0, <a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a> + <a href="ident?i=ULITE_CONTROL">ULITE_CONTROL</a>);
<a name=L238 href="source/linux/drivers/serial/uartlite.c#L238">238</a>         <a href="ident?i=readb">readb</a>(<a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a> + <a href="ident?i=ULITE_CONTROL">ULITE_CONTROL</a>); <b><i>/* dummy */</i></b>
<a name=L239 href="source/linux/drivers/serial/uartlite.c#L239">239</a>         <a href="ident?i=free_irq">free_irq</a>(<a href="ident?i=port">port</a>-&gt;<a href="ident?i=irq">irq</a>, <a href="ident?i=port">port</a>);
<a name=L240 href="source/linux/drivers/serial/uartlite.c#L240">240</a> }
<a name=L241 href="source/linux/drivers/serial/uartlite.c#L241">241</a> 
<a name=L242 href="source/linux/drivers/serial/uartlite.c#L242">242</a> static void <a href="ident?i=ulite_set_termios">ulite_set_termios</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>, struct <a href="ident?i=ktermios">ktermios</a> *<a href="ident?i=termios">termios</a>,
<a name=L243 href="source/linux/drivers/serial/uartlite.c#L243">243</a>                               struct <a href="ident?i=ktermios">ktermios</a> *old)
<a name=L244 href="source/linux/drivers/serial/uartlite.c#L244">244</a> {
<a name=L245 href="source/linux/drivers/serial/uartlite.c#L245">245</a>         unsigned long <a href="ident?i=flags">flags</a>;
<a name=L246 href="source/linux/drivers/serial/uartlite.c#L246">246</a>         unsigned int <a href="ident?i=baud">baud</a>;
<a name=L247 href="source/linux/drivers/serial/uartlite.c#L247">247</a> 
<a name=L248 href="source/linux/drivers/serial/uartlite.c#L248">248</a>         <a href="ident?i=spin_lock_irqsave">spin_lock_irqsave</a>(&amp;<a href="ident?i=port">port</a>-&gt;<a href="ident?i=lock">lock</a>, <a href="ident?i=flags">flags</a>);
<a name=L249 href="source/linux/drivers/serial/uartlite.c#L249">249</a> 
<a name=L250 href="source/linux/drivers/serial/uartlite.c#L250">250</a>         <a href="ident?i=port">port</a>-&gt;read_status_mask = <a href="ident?i=ULITE_STATUS_RXVALID">ULITE_STATUS_RXVALID</a> | <a href="ident?i=ULITE_STATUS_OVERRUN">ULITE_STATUS_OVERRUN</a>
<a name=L251 href="source/linux/drivers/serial/uartlite.c#L251">251</a>                 | <a href="ident?i=ULITE_STATUS_TXFULL">ULITE_STATUS_TXFULL</a>;
<a name=L252 href="source/linux/drivers/serial/uartlite.c#L252">252</a> 
<a name=L253 href="source/linux/drivers/serial/uartlite.c#L253">253</a>         if (<a href="ident?i=termios">termios</a>-&gt;c_iflag &amp; <a href="ident?i=INPCK">INPCK</a>)
<a name=L254 href="source/linux/drivers/serial/uartlite.c#L254">254</a>                 <a href="ident?i=port">port</a>-&gt;read_status_mask |=
<a name=L255 href="source/linux/drivers/serial/uartlite.c#L255">255</a>                         <a href="ident?i=ULITE_STATUS_PARITY">ULITE_STATUS_PARITY</a> | <a href="ident?i=ULITE_STATUS_FRAME">ULITE_STATUS_FRAME</a>;
<a name=L256 href="source/linux/drivers/serial/uartlite.c#L256">256</a> 
<a name=L257 href="source/linux/drivers/serial/uartlite.c#L257">257</a>         <a href="ident?i=port">port</a>-&gt;ignore_status_mask = 0;
<a name=L258 href="source/linux/drivers/serial/uartlite.c#L258">258</a>         if (<a href="ident?i=termios">termios</a>-&gt;c_iflag &amp; <a href="ident?i=IGNPAR">IGNPAR</a>)
<a name=L259 href="source/linux/drivers/serial/uartlite.c#L259">259</a>                 <a href="ident?i=port">port</a>-&gt;ignore_status_mask |= <a href="ident?i=ULITE_STATUS_PARITY">ULITE_STATUS_PARITY</a>
<a name=L260 href="source/linux/drivers/serial/uartlite.c#L260">260</a>                         | <a href="ident?i=ULITE_STATUS_FRAME">ULITE_STATUS_FRAME</a> | <a href="ident?i=ULITE_STATUS_OVERRUN">ULITE_STATUS_OVERRUN</a>;
<a name=L261 href="source/linux/drivers/serial/uartlite.c#L261">261</a> 
<a name=L262 href="source/linux/drivers/serial/uartlite.c#L262">262</a>         <b><i>/* ignore all characters if CREAD is not set */</i></b>
<a name=L263 href="source/linux/drivers/serial/uartlite.c#L263">263</a>         if ((<a href="ident?i=termios">termios</a>-&gt;c_cflag &amp; <a href="ident?i=CREAD">CREAD</a>) == 0)
<a name=L264 href="source/linux/drivers/serial/uartlite.c#L264">264</a>                 <a href="ident?i=port">port</a>-&gt;ignore_status_mask |=
<a name=L265 href="source/linux/drivers/serial/uartlite.c#L265">265</a>                         <a href="ident?i=ULITE_STATUS_RXVALID">ULITE_STATUS_RXVALID</a> | <a href="ident?i=ULITE_STATUS_PARITY">ULITE_STATUS_PARITY</a>
<a name=L266 href="source/linux/drivers/serial/uartlite.c#L266">266</a>                         | <a href="ident?i=ULITE_STATUS_FRAME">ULITE_STATUS_FRAME</a> | <a href="ident?i=ULITE_STATUS_OVERRUN">ULITE_STATUS_OVERRUN</a>;
<a name=L267 href="source/linux/drivers/serial/uartlite.c#L267">267</a> 
<a name=L268 href="source/linux/drivers/serial/uartlite.c#L268">268</a>         <b><i>/* update timeout */</i></b>
<a name=L269 href="source/linux/drivers/serial/uartlite.c#L269">269</a>         <a href="ident?i=baud">baud</a> = <a href="ident?i=uart_get_baud_rate">uart_get_baud_rate</a>(<a href="ident?i=port">port</a>, <a href="ident?i=termios">termios</a>, old, 0, 460800);
<a name=L270 href="source/linux/drivers/serial/uartlite.c#L270">270</a>         <a href="ident?i=uart_update_timeout">uart_update_timeout</a>(<a href="ident?i=port">port</a>, <a href="ident?i=termios">termios</a>-&gt;c_cflag, <a href="ident?i=baud">baud</a>);
<a name=L271 href="source/linux/drivers/serial/uartlite.c#L271">271</a> 
<a name=L272 href="source/linux/drivers/serial/uartlite.c#L272">272</a>         <a href="ident?i=spin_unlock_irqrestore">spin_unlock_irqrestore</a>(&amp;<a href="ident?i=port">port</a>-&gt;<a href="ident?i=lock">lock</a>, <a href="ident?i=flags">flags</a>);
<a name=L273 href="source/linux/drivers/serial/uartlite.c#L273">273</a> }
<a name=L274 href="source/linux/drivers/serial/uartlite.c#L274">274</a> 
<a name=L275 href="source/linux/drivers/serial/uartlite.c#L275">275</a> static const char *<a href="ident?i=ulite_type">ulite_type</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>)
<a name=L276 href="source/linux/drivers/serial/uartlite.c#L276">276</a> {
<a name=L277 href="source/linux/drivers/serial/uartlite.c#L277">277</a>         return <a href="ident?i=port">port</a>-&gt;<a href="ident?i=type">type</a> == <a href="ident?i=PORT_UARTLITE">PORT_UARTLITE</a> ? <i>"uartlite"</i> : <a href="ident?i=NULL">NULL</a>;
<a name=L278 href="source/linux/drivers/serial/uartlite.c#L278">278</a> }
<a name=L279 href="source/linux/drivers/serial/uartlite.c#L279">279</a> 
<a name=L280 href="source/linux/drivers/serial/uartlite.c#L280">280</a> static void <a href="ident?i=ulite_release_port">ulite_release_port</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>)
<a name=L281 href="source/linux/drivers/serial/uartlite.c#L281">281</a> {
<a name=L282 href="source/linux/drivers/serial/uartlite.c#L282">282</a>         <a href="ident?i=release_mem_region">release_mem_region</a>(<a href="ident?i=port">port</a>-&gt;mapbase, <a href="ident?i=ULITE_REGION">ULITE_REGION</a>);
<a name=L283 href="source/linux/drivers/serial/uartlite.c#L283">283</a>         <a href="ident?i=iounmap">iounmap</a>(<a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a>);
<a name=L284 href="source/linux/drivers/serial/uartlite.c#L284">284</a>         <a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a> = <a href="ident?i=NULL">NULL</a>;
<a name=L285 href="source/linux/drivers/serial/uartlite.c#L285">285</a> }
<a name=L286 href="source/linux/drivers/serial/uartlite.c#L286">286</a> 
<a name=L287 href="source/linux/drivers/serial/uartlite.c#L287">287</a> static int <a href="ident?i=ulite_request_port">ulite_request_port</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>)
<a name=L288 href="source/linux/drivers/serial/uartlite.c#L288">288</a> {
<a name=L289 href="source/linux/drivers/serial/uartlite.c#L289">289</a>         <a href="ident?i=pr_debug">pr_debug</a>(<i>"ulite console: port=%p; port-&gt;mapbase=%llx\n"</i>,
<a name=L290 href="source/linux/drivers/serial/uartlite.c#L290">290</a>                  <a href="ident?i=port">port</a>, (unsigned long long) <a href="ident?i=port">port</a>-&gt;mapbase);
<a name=L291 href="source/linux/drivers/serial/uartlite.c#L291">291</a> 
<a name=L292 href="source/linux/drivers/serial/uartlite.c#L292">292</a>         if (!<a href="ident?i=request_mem_region">request_mem_region</a>(<a href="ident?i=port">port</a>-&gt;mapbase, <a href="ident?i=ULITE_REGION">ULITE_REGION</a>, <i>"uartlite"</i>)) {
<a name=L293 href="source/linux/drivers/serial/uartlite.c#L293">293</a>                 <a href="ident?i=dev_err">dev_err</a>(<a href="ident?i=port">port</a>-&gt;<a href="ident?i=dev">dev</a>, <i>"Memory region busy\n"</i>);
<a name=L294 href="source/linux/drivers/serial/uartlite.c#L294">294</a>                 return -<a href="ident?i=EBUSY">EBUSY</a>;
<a name=L295 href="source/linux/drivers/serial/uartlite.c#L295">295</a>         }
<a name=L296 href="source/linux/drivers/serial/uartlite.c#L296">296</a> 
<a name=L297 href="source/linux/drivers/serial/uartlite.c#L297">297</a>         <a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a> = <a href="ident?i=ioremap">ioremap</a>(<a href="ident?i=port">port</a>-&gt;mapbase, <a href="ident?i=ULITE_REGION">ULITE_REGION</a>);
<a name=L298 href="source/linux/drivers/serial/uartlite.c#L298">298</a>         if (!<a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a>) {
<a name=L299 href="source/linux/drivers/serial/uartlite.c#L299">299</a>                 <a href="ident?i=dev_err">dev_err</a>(<a href="ident?i=port">port</a>-&gt;<a href="ident?i=dev">dev</a>, <i>"Unable to map registers\n"</i>);
<a name=L300 href="source/linux/drivers/serial/uartlite.c#L300">300</a>                 <a href="ident?i=release_mem_region">release_mem_region</a>(<a href="ident?i=port">port</a>-&gt;mapbase, <a href="ident?i=ULITE_REGION">ULITE_REGION</a>);
<a name=L301 href="source/linux/drivers/serial/uartlite.c#L301">301</a>                 return -<a href="ident?i=EBUSY">EBUSY</a>;
<a name=L302 href="source/linux/drivers/serial/uartlite.c#L302">302</a>         }
<a name=L303 href="source/linux/drivers/serial/uartlite.c#L303">303</a> 
<a name=L304 href="source/linux/drivers/serial/uartlite.c#L304">304</a>         return 0;
<a name=L305 href="source/linux/drivers/serial/uartlite.c#L305">305</a> }
<a name=L306 href="source/linux/drivers/serial/uartlite.c#L306">306</a> 
<a name=L307 href="source/linux/drivers/serial/uartlite.c#L307">307</a> static void <a href="ident?i=ulite_config_port">ulite_config_port</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>, int <a href="ident?i=flags">flags</a>)
<a name=L308 href="source/linux/drivers/serial/uartlite.c#L308">308</a> {
<a name=L309 href="source/linux/drivers/serial/uartlite.c#L309">309</a>         if (!<a href="ident?i=ulite_request_port">ulite_request_port</a>(<a href="ident?i=port">port</a>))
<a name=L310 href="source/linux/drivers/serial/uartlite.c#L310">310</a>                 <a href="ident?i=port">port</a>-&gt;<a href="ident?i=type">type</a> = <a href="ident?i=PORT_UARTLITE">PORT_UARTLITE</a>;
<a name=L311 href="source/linux/drivers/serial/uartlite.c#L311">311</a> }
<a name=L312 href="source/linux/drivers/serial/uartlite.c#L312">312</a> 
<a name=L313 href="source/linux/drivers/serial/uartlite.c#L313">313</a> static int <a href="ident?i=ulite_verify_port">ulite_verify_port</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>, struct <a href="ident?i=serial_struct">serial_struct</a> *<a href="ident?i=ser">ser</a>)
<a name=L314 href="source/linux/drivers/serial/uartlite.c#L314">314</a> {
<a name=L315 href="source/linux/drivers/serial/uartlite.c#L315">315</a>         <b><i>/* we don't want the core code to modify any port params */</i></b>
<a name=L316 href="source/linux/drivers/serial/uartlite.c#L316">316</a>         return -<a href="ident?i=EINVAL">EINVAL</a>;
<a name=L317 href="source/linux/drivers/serial/uartlite.c#L317">317</a> }
<a name=L318 href="source/linux/drivers/serial/uartlite.c#L318">318</a> 
<a name=L319 href="source/linux/drivers/serial/uartlite.c#L319">319</a> static struct <a href="ident?i=uart_ops">uart_ops</a> <a href="ident?i=ulite_ops">ulite_ops</a> = {
<a name=L320 href="source/linux/drivers/serial/uartlite.c#L320">320</a>         .tx_empty       = <a href="ident?i=ulite_tx_empty">ulite_tx_empty</a>,
<a name=L321 href="source/linux/drivers/serial/uartlite.c#L321">321</a>         .<a href="ident?i=set_mctrl">set_mctrl</a>      = <a href="ident?i=ulite_set_mctrl">ulite_set_mctrl</a>,
<a name=L322 href="source/linux/drivers/serial/uartlite.c#L322">322</a>         .get_mctrl      = <a href="ident?i=ulite_get_mctrl">ulite_get_mctrl</a>,
<a name=L323 href="source/linux/drivers/serial/uartlite.c#L323">323</a>         .stop_tx        = <a href="ident?i=ulite_stop_tx">ulite_stop_tx</a>,
<a name=L324 href="source/linux/drivers/serial/uartlite.c#L324">324</a>         .<a href="ident?i=start_tx">start_tx</a>       = <a href="ident?i=ulite_start_tx">ulite_start_tx</a>,
<a name=L325 href="source/linux/drivers/serial/uartlite.c#L325">325</a>         .stop_rx        = <a href="ident?i=ulite_stop_rx">ulite_stop_rx</a>,
<a name=L326 href="source/linux/drivers/serial/uartlite.c#L326">326</a>         .enable_ms      = <a href="ident?i=ulite_enable_ms">ulite_enable_ms</a>,
<a name=L327 href="source/linux/drivers/serial/uartlite.c#L327">327</a>         .break_ctl      = <a href="ident?i=ulite_break_ctl">ulite_break_ctl</a>,
<a name=L328 href="source/linux/drivers/serial/uartlite.c#L328">328</a>         .<a href="ident?i=startup">startup</a>        = <a href="ident?i=ulite_startup">ulite_startup</a>,
<a name=L329 href="source/linux/drivers/serial/uartlite.c#L329">329</a>         .<a href="ident?i=shutdown">shutdown</a>       = <a href="ident?i=ulite_shutdown">ulite_shutdown</a>,
<a name=L330 href="source/linux/drivers/serial/uartlite.c#L330">330</a>         .<a href="ident?i=set_termios">set_termios</a>    = <a href="ident?i=ulite_set_termios">ulite_set_termios</a>,
<a name=L331 href="source/linux/drivers/serial/uartlite.c#L331">331</a>         .<a href="ident?i=type">type</a>           = <a href="ident?i=ulite_type">ulite_type</a>,
<a name=L332 href="source/linux/drivers/serial/uartlite.c#L332">332</a>         .<a href="ident?i=release_port">release_port</a>   = <a href="ident?i=ulite_release_port">ulite_release_port</a>,
<a name=L333 href="source/linux/drivers/serial/uartlite.c#L333">333</a>         .request_port   = <a href="ident?i=ulite_request_port">ulite_request_port</a>,
<a name=L334 href="source/linux/drivers/serial/uartlite.c#L334">334</a>         .<a href="ident?i=config_port">config_port</a>    = <a href="ident?i=ulite_config_port">ulite_config_port</a>,
<a name=L335 href="source/linux/drivers/serial/uartlite.c#L335">335</a>         .verify_port    = <a href="ident?i=ulite_verify_port">ulite_verify_port</a>
<a name=L336 href="source/linux/drivers/serial/uartlite.c#L336">336</a> };
<a name=L337 href="source/linux/drivers/serial/uartlite.c#L337">337</a> 
<a name=L338 href="source/linux/drivers/serial/uartlite.c#L338">338</a> <b><i>/* ---------------------------------------------------------------------</i></b>
<a name=L339 href="source/linux/drivers/serial/uartlite.c#L339">339</a> <b><i> * Console driver operations</i></b>
<a name=L340 href="source/linux/drivers/serial/uartlite.c#L340">340</a> <b><i> */</i></b>
<a name=L341 href="source/linux/drivers/serial/uartlite.c#L341">341</a> 
<a name=L342 href="source/linux/drivers/serial/uartlite.c#L342">342</a> #ifdef CONFIG_SERIAL_UARTLITE_CONSOLE
<a name=L343 href="source/linux/drivers/serial/uartlite.c#L343">343</a> static void <a href="ident?i=ulite_console_wait_tx">ulite_console_wait_tx</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>)
<a name=L344 href="source/linux/drivers/serial/uartlite.c#L344">344</a> {
<a name=L345 href="source/linux/drivers/serial/uartlite.c#L345">345</a>         int <a href="ident?i=i">i</a>;
<a name=L346 href="source/linux/drivers/serial/uartlite.c#L346">346</a>         <a href="ident?i=u8">u8</a> <a href="ident?i=val">val</a>;
<a name=L347 href="source/linux/drivers/serial/uartlite.c#L347">347</a> 
<a name=L348 href="source/linux/drivers/serial/uartlite.c#L348">348</a>         <b><i>/* Spin waiting for TX fifo to have space available */</i></b>
<a name=L349 href="source/linux/drivers/serial/uartlite.c#L349">349</a>         for (<a href="ident?i=i">i</a> = 0; <a href="ident?i=i">i</a> &lt; 100000; <a href="ident?i=i">i</a>++) {
<a name=L350 href="source/linux/drivers/serial/uartlite.c#L350">350</a>                 <a href="ident?i=val">val</a> = <a href="ident?i=readb">readb</a>(<a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a> + <a href="ident?i=ULITE_STATUS">ULITE_STATUS</a>);
<a name=L351 href="source/linux/drivers/serial/uartlite.c#L351">351</a>                 if ((<a href="ident?i=val">val</a> &amp; <a href="ident?i=ULITE_STATUS_TXFULL">ULITE_STATUS_TXFULL</a>) == 0)
<a name=L352 href="source/linux/drivers/serial/uartlite.c#L352">352</a>                         break;
<a name=L353 href="source/linux/drivers/serial/uartlite.c#L353">353</a>                 <a href="ident?i=cpu_relax">cpu_relax</a>();
<a name=L354 href="source/linux/drivers/serial/uartlite.c#L354">354</a>         }
<a name=L355 href="source/linux/drivers/serial/uartlite.c#L355">355</a> }
<a name=L356 href="source/linux/drivers/serial/uartlite.c#L356">356</a> 
<a name=L357 href="source/linux/drivers/serial/uartlite.c#L357">357</a> static void <a href="ident?i=ulite_console_putchar">ulite_console_putchar</a>(struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>, int <a href="ident?i=ch">ch</a>)
<a name=L358 href="source/linux/drivers/serial/uartlite.c#L358">358</a> {
<a name=L359 href="source/linux/drivers/serial/uartlite.c#L359">359</a>         <a href="ident?i=ulite_console_wait_tx">ulite_console_wait_tx</a>(<a href="ident?i=port">port</a>);
<a name=L360 href="source/linux/drivers/serial/uartlite.c#L360">360</a>         <a href="ident?i=writeb">writeb</a>(<a href="ident?i=ch">ch</a>, <a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a> + <a href="ident?i=ULITE_TX">ULITE_TX</a>);
<a name=L361 href="source/linux/drivers/serial/uartlite.c#L361">361</a> }
<a name=L362 href="source/linux/drivers/serial/uartlite.c#L362">362</a> 
<a name=L363 href="source/linux/drivers/serial/uartlite.c#L363">363</a> static void <a href="ident?i=ulite_console_write">ulite_console_write</a>(struct <a href="ident?i=console">console</a> *co, const char *<a href="ident?i=s">s</a>,
<a name=L364 href="source/linux/drivers/serial/uartlite.c#L364">364</a>                                 unsigned int <a href="ident?i=count">count</a>)
<a name=L365 href="source/linux/drivers/serial/uartlite.c#L365">365</a> {
<a name=L366 href="source/linux/drivers/serial/uartlite.c#L366">366</a>         struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a> = &amp;<a href="ident?i=ulite_ports">ulite_ports</a>[co-&gt;<a href="ident?i=index">index</a>];
<a name=L367 href="source/linux/drivers/serial/uartlite.c#L367">367</a>         unsigned long <a href="ident?i=flags">flags</a>;
<a name=L368 href="source/linux/drivers/serial/uartlite.c#L368">368</a>         unsigned int ier;
<a name=L369 href="source/linux/drivers/serial/uartlite.c#L369">369</a>         int <a href="ident?i=locked">locked</a> = 1;
<a name=L370 href="source/linux/drivers/serial/uartlite.c#L370">370</a> 
<a name=L371 href="source/linux/drivers/serial/uartlite.c#L371">371</a>         if (<a href="ident?i=oops_in_progress">oops_in_progress</a>) {
<a name=L372 href="source/linux/drivers/serial/uartlite.c#L372">372</a>                 <a href="ident?i=locked">locked</a> = <a href="ident?i=spin_trylock_irqsave">spin_trylock_irqsave</a>(&amp;<a href="ident?i=port">port</a>-&gt;<a href="ident?i=lock">lock</a>, <a href="ident?i=flags">flags</a>);
<a name=L373 href="source/linux/drivers/serial/uartlite.c#L373">373</a>         } else
<a name=L374 href="source/linux/drivers/serial/uartlite.c#L374">374</a>                 <a href="ident?i=spin_lock_irqsave">spin_lock_irqsave</a>(&amp;<a href="ident?i=port">port</a>-&gt;<a href="ident?i=lock">lock</a>, <a href="ident?i=flags">flags</a>);
<a name=L375 href="source/linux/drivers/serial/uartlite.c#L375">375</a> 
<a name=L376 href="source/linux/drivers/serial/uartlite.c#L376">376</a>         <b><i>/* save and disable interrupt */</i></b>
<a name=L377 href="source/linux/drivers/serial/uartlite.c#L377">377</a>         ier = <a href="ident?i=readb">readb</a>(<a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a> + <a href="ident?i=ULITE_STATUS">ULITE_STATUS</a>) &amp; <a href="ident?i=ULITE_STATUS_IE">ULITE_STATUS_IE</a>;
<a name=L378 href="source/linux/drivers/serial/uartlite.c#L378">378</a>         <a href="ident?i=writeb">writeb</a>(0, <a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a> + <a href="ident?i=ULITE_CONTROL">ULITE_CONTROL</a>);
<a name=L379 href="source/linux/drivers/serial/uartlite.c#L379">379</a> 
<a name=L380 href="source/linux/drivers/serial/uartlite.c#L380">380</a>         <a href="ident?i=uart_console_write">uart_console_write</a>(<a href="ident?i=port">port</a>, <a href="ident?i=s">s</a>, <a href="ident?i=count">count</a>, <a href="ident?i=ulite_console_putchar">ulite_console_putchar</a>);
<a name=L381 href="source/linux/drivers/serial/uartlite.c#L381">381</a> 
<a name=L382 href="source/linux/drivers/serial/uartlite.c#L382">382</a>         <a href="ident?i=ulite_console_wait_tx">ulite_console_wait_tx</a>(<a href="ident?i=port">port</a>);
<a name=L383 href="source/linux/drivers/serial/uartlite.c#L383">383</a> 
<a name=L384 href="source/linux/drivers/serial/uartlite.c#L384">384</a>         <b><i>/* restore interrupt state */</i></b>
<a name=L385 href="source/linux/drivers/serial/uartlite.c#L385">385</a>         if (ier)
<a name=L386 href="source/linux/drivers/serial/uartlite.c#L386">386</a>                 <a href="ident?i=writeb">writeb</a>(<a href="ident?i=ULITE_CONTROL_IE">ULITE_CONTROL_IE</a>, <a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a> + <a href="ident?i=ULITE_CONTROL">ULITE_CONTROL</a>);
<a name=L387 href="source/linux/drivers/serial/uartlite.c#L387">387</a> 
<a name=L388 href="source/linux/drivers/serial/uartlite.c#L388">388</a>         if (<a href="ident?i=locked">locked</a>)
<a name=L389 href="source/linux/drivers/serial/uartlite.c#L389">389</a>                 <a href="ident?i=spin_unlock_irqrestore">spin_unlock_irqrestore</a>(&amp;<a href="ident?i=port">port</a>-&gt;<a href="ident?i=lock">lock</a>, <a href="ident?i=flags">flags</a>);
<a name=L390 href="source/linux/drivers/serial/uartlite.c#L390">390</a> }
<a name=L391 href="source/linux/drivers/serial/uartlite.c#L391">391</a> 
<a name=L392 href="source/linux/drivers/serial/uartlite.c#L392">392</a> static int <a href="ident?i=__init">__init</a> <a href="ident?i=ulite_console_setup">ulite_console_setup</a>(struct <a href="ident?i=console">console</a> *co, char *<a href="ident?i=options">options</a>)
<a name=L393 href="source/linux/drivers/serial/uartlite.c#L393">393</a> {
<a name=L394 href="source/linux/drivers/serial/uartlite.c#L394">394</a>         struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>;
<a name=L395 href="source/linux/drivers/serial/uartlite.c#L395">395</a>         int <a href="ident?i=baud">baud</a> = 9600;
<a name=L396 href="source/linux/drivers/serial/uartlite.c#L396">396</a>         int <a href="ident?i=bits">bits</a> = 8;
<a name=L397 href="source/linux/drivers/serial/uartlite.c#L397">397</a>         int <a href="ident?i=parity">parity</a> = <i>'n'</i>;
<a name=L398 href="source/linux/drivers/serial/uartlite.c#L398">398</a>         int flow = <i>'n'</i>;
<a name=L399 href="source/linux/drivers/serial/uartlite.c#L399">399</a> 
<a name=L400 href="source/linux/drivers/serial/uartlite.c#L400">400</a>         if (co-&gt;<a href="ident?i=index">index</a> &lt; 0 || co-&gt;<a href="ident?i=index">index</a> &gt;= <a href="ident?i=ULITE_NR_UARTS">ULITE_NR_UARTS</a>)
<a name=L401 href="source/linux/drivers/serial/uartlite.c#L401">401</a>                 return -<a href="ident?i=EINVAL">EINVAL</a>;
<a name=L402 href="source/linux/drivers/serial/uartlite.c#L402">402</a> 
<a name=L403 href="source/linux/drivers/serial/uartlite.c#L403">403</a>         <a href="ident?i=port">port</a> = &amp;<a href="ident?i=ulite_ports">ulite_ports</a>[co-&gt;<a href="ident?i=index">index</a>];
<a name=L404 href="source/linux/drivers/serial/uartlite.c#L404">404</a> 
<a name=L405 href="source/linux/drivers/serial/uartlite.c#L405">405</a>         <b><i>/* Has the device been initialized yet? */</i></b>
<a name=L406 href="source/linux/drivers/serial/uartlite.c#L406">406</a>         if (!<a href="ident?i=port">port</a>-&gt;mapbase) {
<a name=L407 href="source/linux/drivers/serial/uartlite.c#L407">407</a>                 <a href="ident?i=pr_debug">pr_debug</a>(<i>"console on ttyUL%i not present\n"</i>, co-&gt;<a href="ident?i=index">index</a>);
<a name=L408 href="source/linux/drivers/serial/uartlite.c#L408">408</a>                 return -<a href="ident?i=ENODEV">ENODEV</a>;
<a name=L409 href="source/linux/drivers/serial/uartlite.c#L409">409</a>         }
<a name=L410 href="source/linux/drivers/serial/uartlite.c#L410">410</a> 
<a name=L411 href="source/linux/drivers/serial/uartlite.c#L411">411</a>         <b><i>/* not initialized yet? */</i></b>
<a name=L412 href="source/linux/drivers/serial/uartlite.c#L412">412</a>         if (!<a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a>) {
<a name=L413 href="source/linux/drivers/serial/uartlite.c#L413">413</a>                 if (<a href="ident?i=ulite_request_port">ulite_request_port</a>(<a href="ident?i=port">port</a>))
<a name=L414 href="source/linux/drivers/serial/uartlite.c#L414">414</a>                         return -<a href="ident?i=ENODEV">ENODEV</a>;
<a name=L415 href="source/linux/drivers/serial/uartlite.c#L415">415</a>         }
<a name=L416 href="source/linux/drivers/serial/uartlite.c#L416">416</a> 
<a name=L417 href="source/linux/drivers/serial/uartlite.c#L417">417</a>         if (<a href="ident?i=options">options</a>)
<a name=L418 href="source/linux/drivers/serial/uartlite.c#L418">418</a>                 <a href="ident?i=uart_parse_options">uart_parse_options</a>(<a href="ident?i=options">options</a>, &amp;<a href="ident?i=baud">baud</a>, &amp;<a href="ident?i=parity">parity</a>, &amp;<a href="ident?i=bits">bits</a>, &amp;flow);
<a name=L419 href="source/linux/drivers/serial/uartlite.c#L419">419</a> 
<a name=L420 href="source/linux/drivers/serial/uartlite.c#L420">420</a>         return <a href="ident?i=uart_set_options">uart_set_options</a>(<a href="ident?i=port">port</a>, co, <a href="ident?i=baud">baud</a>, <a href="ident?i=parity">parity</a>, <a href="ident?i=bits">bits</a>, flow);
<a name=L421 href="source/linux/drivers/serial/uartlite.c#L421">421</a> }
<a name=L422 href="source/linux/drivers/serial/uartlite.c#L422">422</a> 
<a name=L423 href="source/linux/drivers/serial/uartlite.c#L423">423</a> static struct <a href="ident?i=uart_driver">uart_driver</a> <a href="ident?i=ulite_uart_driver">ulite_uart_driver</a>;
<a name=L424 href="source/linux/drivers/serial/uartlite.c#L424">424</a> 
<a name=L425 href="source/linux/drivers/serial/uartlite.c#L425">425</a> static struct <a href="ident?i=console">console</a> <a href="ident?i=ulite_console">ulite_console</a> = {
<a name=L426 href="source/linux/drivers/serial/uartlite.c#L426">426</a>         .<a href="ident?i=name">name</a>   = <a href="ident?i=ULITE_NAME">ULITE_NAME</a>,
<a name=L427 href="source/linux/drivers/serial/uartlite.c#L427">427</a>         .<a href="ident?i=write">write</a>  = <a href="ident?i=ulite_console_write">ulite_console_write</a>,
<a name=L428 href="source/linux/drivers/serial/uartlite.c#L428">428</a>         .<a href="ident?i=device">device</a> = <a href="ident?i=uart_console_device">uart_console_device</a>,
<a name=L429 href="source/linux/drivers/serial/uartlite.c#L429">429</a>         .<a href="ident?i=setup">setup</a>  = <a href="ident?i=ulite_console_setup">ulite_console_setup</a>,
<a name=L430 href="source/linux/drivers/serial/uartlite.c#L430">430</a>         .<a href="ident?i=flags">flags</a>  = <a href="ident?i=CON_PRINTBUFFER">CON_PRINTBUFFER</a>,
<a name=L431 href="source/linux/drivers/serial/uartlite.c#L431">431</a>         .<a href="ident?i=index">index</a>  = -1, <b><i>/* Specified on the cmdline (e.g. console=ttyUL0 ) */</i></b>
<a name=L432 href="source/linux/drivers/serial/uartlite.c#L432">432</a>         .<a href="ident?i=data">data</a>   = &amp;<a href="ident?i=ulite_uart_driver">ulite_uart_driver</a>,
<a name=L433 href="source/linux/drivers/serial/uartlite.c#L433">433</a> };
<a name=L434 href="source/linux/drivers/serial/uartlite.c#L434">434</a> 
<a name=L435 href="source/linux/drivers/serial/uartlite.c#L435">435</a> static int <a href="ident?i=__init">__init</a> <a href="ident?i=ulite_console_init">ulite_console_init</a>(void)
<a name=L436 href="source/linux/drivers/serial/uartlite.c#L436">436</a> {
<a name=L437 href="source/linux/drivers/serial/uartlite.c#L437">437</a>         <a href="ident?i=register_console">register_console</a>(&amp;<a href="ident?i=ulite_console">ulite_console</a>);
<a name=L438 href="source/linux/drivers/serial/uartlite.c#L438">438</a>         return 0;
<a name=L439 href="source/linux/drivers/serial/uartlite.c#L439">439</a> }
<a name=L440 href="source/linux/drivers/serial/uartlite.c#L440">440</a> 
<a name=L441 href="source/linux/drivers/serial/uartlite.c#L441">441</a> <a href="ident?i=console_initcall">console_initcall</a>(<a href="ident?i=ulite_console_init">ulite_console_init</a>);
<a name=L442 href="source/linux/drivers/serial/uartlite.c#L442">442</a> 
<a name=L443 href="source/linux/drivers/serial/uartlite.c#L443">443</a> #endif <b><i>/* CONFIG_SERIAL_UARTLITE_CONSOLE */</i></b>
<a name=L444 href="source/linux/drivers/serial/uartlite.c#L444">444</a> 
<a name=L445 href="source/linux/drivers/serial/uartlite.c#L445">445</a> static struct <a href="ident?i=uart_driver">uart_driver</a> <a href="ident?i=ulite_uart_driver">ulite_uart_driver</a> = {
<a name=L446 href="source/linux/drivers/serial/uartlite.c#L446">446</a>         .owner          = <a href="ident?i=THIS_MODULE">THIS_MODULE</a>,
<a name=L447 href="source/linux/drivers/serial/uartlite.c#L447">447</a>         .<a href="ident?i=driver_name">driver_name</a>    = <i>"uartlite"</i>,
<a name=L448 href="source/linux/drivers/serial/uartlite.c#L448">448</a>         .<a href="ident?i=dev_name">dev_name</a>       = <a href="ident?i=ULITE_NAME">ULITE_NAME</a>,
<a name=L449 href="source/linux/drivers/serial/uartlite.c#L449">449</a>         .<a href="ident?i=major">major</a>          = <a href="ident?i=ULITE_MAJOR">ULITE_MAJOR</a>,
<a name=L450 href="source/linux/drivers/serial/uartlite.c#L450">450</a>         .<a href="ident?i=minor">minor</a>          = <a href="ident?i=ULITE_MINOR">ULITE_MINOR</a>,
<a name=L451 href="source/linux/drivers/serial/uartlite.c#L451">451</a>         .nr             = <a href="ident?i=ULITE_NR_UARTS">ULITE_NR_UARTS</a>,
<a name=L452 href="source/linux/drivers/serial/uartlite.c#L452">452</a> #ifdef CONFIG_SERIAL_UARTLITE_CONSOLE
<a name=L453 href="source/linux/drivers/serial/uartlite.c#L453">453</a>         .cons           = &amp;<a href="ident?i=ulite_console">ulite_console</a>,
<a name=L454 href="source/linux/drivers/serial/uartlite.c#L454">454</a> #endif
<a name=L455 href="source/linux/drivers/serial/uartlite.c#L455">455</a> };
<a name=L456 href="source/linux/drivers/serial/uartlite.c#L456">456</a> 
<a name=L457 href="source/linux/drivers/serial/uartlite.c#L457">457</a> <b><i>/* ---------------------------------------------------------------------</i></b>
<a name=L458 href="source/linux/drivers/serial/uartlite.c#L458">458</a> <b><i> * Port assignment functions (mapping devices to uart_port structures)</i></b>
<a name=L459 href="source/linux/drivers/serial/uartlite.c#L459">459</a> <b><i> */</i></b>
<a name=L460 href="source/linux/drivers/serial/uartlite.c#L460">460</a> 
<a name=L461 href="source/linux/drivers/serial/uartlite.c#L461">461</a> <b><i>/** ulite_assign: register a uartlite device with the driver</i></b>
<a name=L462 href="source/linux/drivers/serial/uartlite.c#L462">462</a> <b><i> *</i></b>
<a name=L463 href="source/linux/drivers/serial/uartlite.c#L463">463</a> <b><i> * @dev: pointer to device structure</i></b>
<a name=L464 href="source/linux/drivers/serial/uartlite.c#L464">464</a> <b><i> * @id: requested id number.  Pass -1 for automatic port assignment</i></b>
<a name=L465 href="source/linux/drivers/serial/uartlite.c#L465">465</a> <b><i> * @base: base address of uartlite registers</i></b>
<a name=L466 href="source/linux/drivers/serial/uartlite.c#L466">466</a> <b><i> * @irq: irq number for uartlite</i></b>
<a name=L467 href="source/linux/drivers/serial/uartlite.c#L467">467</a> <b><i> *</i></b>
<a name=L468 href="source/linux/drivers/serial/uartlite.c#L468">468</a> <b><i> * Returns: 0 on success, &lt;0 otherwise</i></b>
<a name=L469 href="source/linux/drivers/serial/uartlite.c#L469">469</a> <b><i> */</i></b>
<a name=L470 href="source/linux/drivers/serial/uartlite.c#L470">470</a> static int <a href="ident?i=__devinit">__devinit</a> <a href="ident?i=ulite_assign">ulite_assign</a>(struct <a href="ident?i=device">device</a> *<a href="ident?i=dev">dev</a>, int <a href="ident?i=id">id</a>, <a href="ident?i=u32">u32</a> <a href="ident?i=base">base</a>, int <a href="ident?i=irq">irq</a>)
<a name=L471 href="source/linux/drivers/serial/uartlite.c#L471">471</a> {
<a name=L472 href="source/linux/drivers/serial/uartlite.c#L472">472</a>         struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a>;
<a name=L473 href="source/linux/drivers/serial/uartlite.c#L473">473</a>         int <a href="ident?i=rc">rc</a>;
<a name=L474 href="source/linux/drivers/serial/uartlite.c#L474">474</a> 
<a name=L475 href="source/linux/drivers/serial/uartlite.c#L475">475</a>         <b><i>/* if id = -1; then scan for a free id and use that */</i></b>
<a name=L476 href="source/linux/drivers/serial/uartlite.c#L476">476</a>         if (<a href="ident?i=id">id</a> &lt; 0) {
<a name=L477 href="source/linux/drivers/serial/uartlite.c#L477">477</a>                 for (<a href="ident?i=id">id</a> = 0; <a href="ident?i=id">id</a> &lt; <a href="ident?i=ULITE_NR_UARTS">ULITE_NR_UARTS</a>; <a href="ident?i=id">id</a>++)
<a name=L478 href="source/linux/drivers/serial/uartlite.c#L478">478</a>                         if (<a href="ident?i=ulite_ports">ulite_ports</a>[<a href="ident?i=id">id</a>].mapbase == 0)
<a name=L479 href="source/linux/drivers/serial/uartlite.c#L479">479</a>                                 break;
<a name=L480 href="source/linux/drivers/serial/uartlite.c#L480">480</a>         }
<a name=L481 href="source/linux/drivers/serial/uartlite.c#L481">481</a>         if (<a href="ident?i=id">id</a> &lt; 0 || <a href="ident?i=id">id</a> &gt;= <a href="ident?i=ULITE_NR_UARTS">ULITE_NR_UARTS</a>) {
<a name=L482 href="source/linux/drivers/serial/uartlite.c#L482">482</a>                 <a href="ident?i=dev_err">dev_err</a>(<a href="ident?i=dev">dev</a>, <i>"%s%i too large\n"</i>, <a href="ident?i=ULITE_NAME">ULITE_NAME</a>, <a href="ident?i=id">id</a>);
<a name=L483 href="source/linux/drivers/serial/uartlite.c#L483">483</a>                 return -<a href="ident?i=EINVAL">EINVAL</a>;
<a name=L484 href="source/linux/drivers/serial/uartlite.c#L484">484</a>         }
<a name=L485 href="source/linux/drivers/serial/uartlite.c#L485">485</a> 
<a name=L486 href="source/linux/drivers/serial/uartlite.c#L486">486</a>         if ((<a href="ident?i=ulite_ports">ulite_ports</a>[<a href="ident?i=id">id</a>].mapbase) &amp;&amp; (<a href="ident?i=ulite_ports">ulite_ports</a>[<a href="ident?i=id">id</a>].mapbase != <a href="ident?i=base">base</a>)) {
<a name=L487 href="source/linux/drivers/serial/uartlite.c#L487">487</a>                 <a href="ident?i=dev_err">dev_err</a>(<a href="ident?i=dev">dev</a>, <i>"cannot assign to %s%i; it is already in use\n"</i>,
<a name=L488 href="source/linux/drivers/serial/uartlite.c#L488">488</a>                         <a href="ident?i=ULITE_NAME">ULITE_NAME</a>, <a href="ident?i=id">id</a>);
<a name=L489 href="source/linux/drivers/serial/uartlite.c#L489">489</a>                 return -<a href="ident?i=EBUSY">EBUSY</a>;
<a name=L490 href="source/linux/drivers/serial/uartlite.c#L490">490</a>         }
<a name=L491 href="source/linux/drivers/serial/uartlite.c#L491">491</a> 
<a name=L492 href="source/linux/drivers/serial/uartlite.c#L492">492</a>         <a href="ident?i=port">port</a> = &amp;<a href="ident?i=ulite_ports">ulite_ports</a>[<a href="ident?i=id">id</a>];
<a name=L493 href="source/linux/drivers/serial/uartlite.c#L493">493</a> 
<a name=L494 href="source/linux/drivers/serial/uartlite.c#L494">494</a>         <a href="ident?i=spin_lock_init">spin_lock_init</a>(&amp;<a href="ident?i=port">port</a>-&gt;<a href="ident?i=lock">lock</a>);
<a name=L495 href="source/linux/drivers/serial/uartlite.c#L495">495</a>         <a href="ident?i=port">port</a>-&gt;fifosize = 16;
<a name=L496 href="source/linux/drivers/serial/uartlite.c#L496">496</a>         <a href="ident?i=port">port</a>-&gt;regshift = 2;
<a name=L497 href="source/linux/drivers/serial/uartlite.c#L497">497</a>         <a href="ident?i=port">port</a>-&gt;iotype = <a href="ident?i=UPIO_MEM">UPIO_MEM</a>;
<a name=L498 href="source/linux/drivers/serial/uartlite.c#L498">498</a>         <a href="ident?i=port">port</a>-&gt;<a href="ident?i=iobase">iobase</a> = 1; <b><i>/* mark port in use */</i></b>
<a name=L499 href="source/linux/drivers/serial/uartlite.c#L499">499</a>         <a href="ident?i=port">port</a>-&gt;mapbase = <a href="ident?i=base">base</a>;
<a name=L500 href="source/linux/drivers/serial/uartlite.c#L500">500</a>         <a href="ident?i=port">port</a>-&gt;<a href="ident?i=membase">membase</a> = <a href="ident?i=NULL">NULL</a>;
<a name=L501 href="source/linux/drivers/serial/uartlite.c#L501">501</a>         <a href="ident?i=port">port</a>-&gt;<a href="ident?i=ops">ops</a> = &amp;<a href="ident?i=ulite_ops">ulite_ops</a>;
<a name=L502 href="source/linux/drivers/serial/uartlite.c#L502">502</a>         <a href="ident?i=port">port</a>-&gt;<a href="ident?i=irq">irq</a> = <a href="ident?i=irq">irq</a>;
<a name=L503 href="source/linux/drivers/serial/uartlite.c#L503">503</a>         <a href="ident?i=port">port</a>-&gt;<a href="ident?i=flags">flags</a> = <a href="ident?i=UPF_BOOT_AUTOCONF">UPF_BOOT_AUTOCONF</a>;
<a name=L504 href="source/linux/drivers/serial/uartlite.c#L504">504</a>         <a href="ident?i=port">port</a>-&gt;<a href="ident?i=dev">dev</a> = <a href="ident?i=dev">dev</a>;
<a name=L505 href="source/linux/drivers/serial/uartlite.c#L505">505</a>         <a href="ident?i=port">port</a>-&gt;<a href="ident?i=type">type</a> = <a href="ident?i=PORT_UNKNOWN">PORT_UNKNOWN</a>;
<a name=L506 href="source/linux/drivers/serial/uartlite.c#L506">506</a>         <a href="ident?i=port">port</a>-&gt;<a href="ident?i=line">line</a> = <a href="ident?i=id">id</a>;
<a name=L507 href="source/linux/drivers/serial/uartlite.c#L507">507</a> 
<a name=L508 href="source/linux/drivers/serial/uartlite.c#L508">508</a>         <a href="ident?i=dev_set_drvdata">dev_set_drvdata</a>(<a href="ident?i=dev">dev</a>, <a href="ident?i=port">port</a>);
<a name=L509 href="source/linux/drivers/serial/uartlite.c#L509">509</a> 
<a name=L510 href="source/linux/drivers/serial/uartlite.c#L510">510</a>         <b><i>/* Register the port */</i></b>
<a name=L511 href="source/linux/drivers/serial/uartlite.c#L511">511</a>         <a href="ident?i=rc">rc</a> = <a href="ident?i=uart_add_one_port">uart_add_one_port</a>(&amp;<a href="ident?i=ulite_uart_driver">ulite_uart_driver</a>, <a href="ident?i=port">port</a>);
<a name=L512 href="source/linux/drivers/serial/uartlite.c#L512">512</a>         if (<a href="ident?i=rc">rc</a>) {
<a name=L513 href="source/linux/drivers/serial/uartlite.c#L513">513</a>                 <a href="ident?i=dev_err">dev_err</a>(<a href="ident?i=dev">dev</a>, <i>"uart_add_one_port() failed; err=%i\n"</i>, <a href="ident?i=rc">rc</a>);
<a name=L514 href="source/linux/drivers/serial/uartlite.c#L514">514</a>                 <a href="ident?i=port">port</a>-&gt;mapbase = 0;
<a name=L515 href="source/linux/drivers/serial/uartlite.c#L515">515</a>                 <a href="ident?i=dev_set_drvdata">dev_set_drvdata</a>(<a href="ident?i=dev">dev</a>, <a href="ident?i=NULL">NULL</a>);
<a name=L516 href="source/linux/drivers/serial/uartlite.c#L516">516</a>                 return <a href="ident?i=rc">rc</a>;
<a name=L517 href="source/linux/drivers/serial/uartlite.c#L517">517</a>         }
<a name=L518 href="source/linux/drivers/serial/uartlite.c#L518">518</a> 
<a name=L519 href="source/linux/drivers/serial/uartlite.c#L519">519</a>         return 0;
<a name=L520 href="source/linux/drivers/serial/uartlite.c#L520">520</a> }
<a name=L521 href="source/linux/drivers/serial/uartlite.c#L521">521</a> 
<a name=L522 href="source/linux/drivers/serial/uartlite.c#L522">522</a> <b><i>/** ulite_release: register a uartlite device with the driver</i></b>
<a name=L523 href="source/linux/drivers/serial/uartlite.c#L523">523</a> <b><i> *</i></b>
<a name=L524 href="source/linux/drivers/serial/uartlite.c#L524">524</a> <b><i> * @dev: pointer to device structure</i></b>
<a name=L525 href="source/linux/drivers/serial/uartlite.c#L525">525</a> <b><i> */</i></b>
<a name=L526 href="source/linux/drivers/serial/uartlite.c#L526">526</a> static int <a href="ident?i=__devexit">__devexit</a> <a href="ident?i=ulite_release">ulite_release</a>(struct <a href="ident?i=device">device</a> *<a href="ident?i=dev">dev</a>)
<a name=L527 href="source/linux/drivers/serial/uartlite.c#L527">527</a> {
<a name=L528 href="source/linux/drivers/serial/uartlite.c#L528">528</a>         struct <a href="ident?i=uart_port">uart_port</a> *<a href="ident?i=port">port</a> = <a href="ident?i=dev_get_drvdata">dev_get_drvdata</a>(<a href="ident?i=dev">dev</a>);
<a name=L529 href="source/linux/drivers/serial/uartlite.c#L529">529</a>         int <a href="ident?i=rc">rc</a> = 0;
<a name=L530 href="source/linux/drivers/serial/uartlite.c#L530">530</a> 
<a name=L531 href="source/linux/drivers/serial/uartlite.c#L531">531</a>         if (<a href="ident?i=port">port</a>) {
<a name=L532 href="source/linux/drivers/serial/uartlite.c#L532">532</a>                 <a href="ident?i=rc">rc</a> = <a href="ident?i=uart_remove_one_port">uart_remove_one_port</a>(&amp;<a href="ident?i=ulite_uart_driver">ulite_uart_driver</a>, <a href="ident?i=port">port</a>);
<a name=L533 href="source/linux/drivers/serial/uartlite.c#L533">533</a>                 <a href="ident?i=dev_set_drvdata">dev_set_drvdata</a>(<a href="ident?i=dev">dev</a>, <a href="ident?i=NULL">NULL</a>);
<a name=L534 href="source/linux/drivers/serial/uartlite.c#L534">534</a>                 <a href="ident?i=port">port</a>-&gt;mapbase = 0;
<a name=L535 href="source/linux/drivers/serial/uartlite.c#L535">535</a>         }
<a name=L536 href="source/linux/drivers/serial/uartlite.c#L536">536</a> 
<a name=L537 href="source/linux/drivers/serial/uartlite.c#L537">537</a>         return <a href="ident?i=rc">rc</a>;
<a name=L538 href="source/linux/drivers/serial/uartlite.c#L538">538</a> }
<a name=L539 href="source/linux/drivers/serial/uartlite.c#L539">539</a> 
<a name=L540 href="source/linux/drivers/serial/uartlite.c#L540">540</a> <b><i>/* ---------------------------------------------------------------------</i></b>
<a name=L541 href="source/linux/drivers/serial/uartlite.c#L541">541</a> <b><i> * Platform bus binding</i></b>
<a name=L542 href="source/linux/drivers/serial/uartlite.c#L542">542</a> <b><i> */</i></b>
<a name=L543 href="source/linux/drivers/serial/uartlite.c#L543">543</a> 
<a name=L544 href="source/linux/drivers/serial/uartlite.c#L544">544</a> static int <a href="ident?i=__devinit">__devinit</a> <a href="ident?i=ulite_probe">ulite_probe</a>(struct <a href="ident?i=platform_device">platform_device</a> *<a href="ident?i=pdev">pdev</a>)
<a name=L545 href="source/linux/drivers/serial/uartlite.c#L545">545</a> {
<a name=L546 href="source/linux/drivers/serial/uartlite.c#L546">546</a>         struct <a href="ident?i=resource">resource</a> *<a href="ident?i=res">res</a>, *res2;
<a name=L547 href="source/linux/drivers/serial/uartlite.c#L547">547</a> 
<a name=L548 href="source/linux/drivers/serial/uartlite.c#L548">548</a>         <a href="ident?i=res">res</a> = <a href="ident?i=platform_get_resource">platform_get_resource</a>(<a href="ident?i=pdev">pdev</a>, <a href="ident?i=IORESOURCE_MEM">IORESOURCE_MEM</a>, 0);
<a name=L549 href="source/linux/drivers/serial/uartlite.c#L549">549</a>         if (!<a href="ident?i=res">res</a>)
<a name=L550 href="source/linux/drivers/serial/uartlite.c#L550">550</a>                 return -<a href="ident?i=ENODEV">ENODEV</a>;
<a name=L551 href="source/linux/drivers/serial/uartlite.c#L551">551</a> 
<a name=L552 href="source/linux/drivers/serial/uartlite.c#L552">552</a>         res2 = <a href="ident?i=platform_get_resource">platform_get_resource</a>(<a href="ident?i=pdev">pdev</a>, <a href="ident?i=IORESOURCE_IRQ">IORESOURCE_IRQ</a>, 0);
<a name=L553 href="source/linux/drivers/serial/uartlite.c#L553">553</a>         if (!res2)
<a name=L554 href="source/linux/drivers/serial/uartlite.c#L554">554</a>                 return -<a href="ident?i=ENODEV">ENODEV</a>;
<a name=L555 href="source/linux/drivers/serial/uartlite.c#L555">555</a> 
<a name=L556 href="source/linux/drivers/serial/uartlite.c#L556">556</a>         return <a href="ident?i=ulite_assign">ulite_assign</a>(&amp;<a href="ident?i=pdev">pdev</a>-&gt;<a href="ident?i=dev">dev</a>, <a href="ident?i=pdev">pdev</a>-&gt;<a href="ident?i=id">id</a>, <a href="ident?i=res">res</a>-&gt;<a href="ident?i=start">start</a>, res2-&gt;<a href="ident?i=start">start</a>);
<a name=L557 href="source/linux/drivers/serial/uartlite.c#L557">557</a> }
<a name=L558 href="source/linux/drivers/serial/uartlite.c#L558">558</a> 
<a name=L559 href="source/linux/drivers/serial/uartlite.c#L559">559</a> static int <a href="ident?i=__devexit">__devexit</a> <a href="ident?i=ulite_remove">ulite_remove</a>(struct <a href="ident?i=platform_device">platform_device</a> *<a href="ident?i=pdev">pdev</a>)
<a name=L560 href="source/linux/drivers/serial/uartlite.c#L560">560</a> {
<a name=L561 href="source/linux/drivers/serial/uartlite.c#L561">561</a>         return <a href="ident?i=ulite_release">ulite_release</a>(&amp;<a href="ident?i=pdev">pdev</a>-&gt;<a href="ident?i=dev">dev</a>);
<a name=L562 href="source/linux/drivers/serial/uartlite.c#L562">562</a> }
<a name=L563 href="source/linux/drivers/serial/uartlite.c#L563">563</a> 
<a name=L564 href="source/linux/drivers/serial/uartlite.c#L564">564</a> <b><i>/* work with hotplug and coldplug */</i></b>
<a name=L565 href="source/linux/drivers/serial/uartlite.c#L565">565</a> <a href="ident?i=MODULE_ALIAS">MODULE_ALIAS</a>(<i>"platform:uartlite"</i>);
<a name=L566 href="source/linux/drivers/serial/uartlite.c#L566">566</a> 
<a name=L567 href="source/linux/drivers/serial/uartlite.c#L567">567</a> static struct <a href="ident?i=platform_driver">platform_driver</a> <a href="ident?i=ulite_platform_driver">ulite_platform_driver</a> = {
<a name=L568 href="source/linux/drivers/serial/uartlite.c#L568">568</a>         .<a href="ident?i=probe">probe</a>  = <a href="ident?i=ulite_probe">ulite_probe</a>,
<a name=L569 href="source/linux/drivers/serial/uartlite.c#L569">569</a>         .<a href="ident?i=remove">remove</a> = <a href="ident?i=__devexit_p">__devexit_p</a>(<a href="ident?i=ulite_remove">ulite_remove</a>),
<a name=L570 href="source/linux/drivers/serial/uartlite.c#L570">570</a>         .<a href="ident?i=driver">driver</a> = {
<a name=L571 href="source/linux/drivers/serial/uartlite.c#L571">571</a>                    .owner = <a href="ident?i=THIS_MODULE">THIS_MODULE</a>,
<a name=L572 href="source/linux/drivers/serial/uartlite.c#L572">572</a>                    .<a href="ident?i=name">name</a>  = <i>"uartlite"</i>,
<a name=L573 href="source/linux/drivers/serial/uartlite.c#L573">573</a>                    },
<a name=L574 href="source/linux/drivers/serial/uartlite.c#L574">574</a> };
<a name=L575 href="source/linux/drivers/serial/uartlite.c#L575">575</a> 
<a name=L576 href="source/linux/drivers/serial/uartlite.c#L576">576</a> <b><i>/* ---------------------------------------------------------------------</i></b>
<a name=L577 href="source/linux/drivers/serial/uartlite.c#L577">577</a> <b><i> * OF bus bindings</i></b>
<a name=L578 href="source/linux/drivers/serial/uartlite.c#L578">578</a> <b><i> */</i></b>
<a name=L579 href="source/linux/drivers/serial/uartlite.c#L579">579</a> #if defined(CONFIG_OF)
<a name=L580 href="source/linux/drivers/serial/uartlite.c#L580">580</a> static int <a href="ident?i=__devinit">__devinit</a>
<a name=L581 href="source/linux/drivers/serial/uartlite.c#L581">581</a> <a href="ident?i=ulite_of_probe">ulite_of_probe</a>(struct <a href="ident?i=of_device">of_device</a> *<a href="ident?i=op">op</a>, const struct <a href="ident?i=of_device_id">of_device_id</a> *<a href="ident?i=match">match</a>)
<a name=L582 href="source/linux/drivers/serial/uartlite.c#L582">582</a> {
<a name=L583 href="source/linux/drivers/serial/uartlite.c#L583">583</a>         struct <a href="ident?i=resource">resource</a> <a href="ident?i=res">res</a>;
<a name=L584 href="source/linux/drivers/serial/uartlite.c#L584">584</a>         const unsigned int *<a href="ident?i=id">id</a>;
<a name=L585 href="source/linux/drivers/serial/uartlite.c#L585">585</a>         int <a href="ident?i=irq">irq</a>, <a href="ident?i=rc">rc</a>;
<a name=L586 href="source/linux/drivers/serial/uartlite.c#L586">586</a> 
<a name=L587 href="source/linux/drivers/serial/uartlite.c#L587">587</a>         <a href="ident?i=dev_dbg">dev_dbg</a>(&amp;<a href="ident?i=op">op</a>-&gt;<a href="ident?i=dev">dev</a>, <i>"%s(%p, %p)\n"</i>, <a href="ident?i=__func__">__func__</a>, <a href="ident?i=op">op</a>, <a href="ident?i=match">match</a>);
<a name=L588 href="source/linux/drivers/serial/uartlite.c#L588">588</a> 
<a name=L589 href="source/linux/drivers/serial/uartlite.c#L589">589</a>         <a href="ident?i=rc">rc</a> = of_address_to_resource(<a href="ident?i=op">op</a>-&gt;<a href="ident?i=node">node</a>, 0, &amp;<a href="ident?i=res">res</a>);
<a name=L590 href="source/linux/drivers/serial/uartlite.c#L590">590</a>         if (<a href="ident?i=rc">rc</a>) {
<a name=L591 href="source/linux/drivers/serial/uartlite.c#L591">591</a>                 <a href="ident?i=dev_err">dev_err</a>(&amp;<a href="ident?i=op">op</a>-&gt;<a href="ident?i=dev">dev</a>, <i>"invalid address\n"</i>);
<a name=L592 href="source/linux/drivers/serial/uartlite.c#L592">592</a>                 return <a href="ident?i=rc">rc</a>;
<a name=L593 href="source/linux/drivers/serial/uartlite.c#L593">593</a>         }
<a name=L594 href="source/linux/drivers/serial/uartlite.c#L594">594</a> 
<a name=L595 href="source/linux/drivers/serial/uartlite.c#L595">595</a>         <a href="ident?i=irq">irq</a> = irq_of_parse_and_map(<a href="ident?i=op">op</a>-&gt;<a href="ident?i=node">node</a>, 0);
<a name=L596 href="source/linux/drivers/serial/uartlite.c#L596">596</a> 
<a name=L597 href="source/linux/drivers/serial/uartlite.c#L597">597</a>         <a href="ident?i=id">id</a> = <a href="ident?i=of_get_property">of_get_property</a>(<a href="ident?i=op">op</a>-&gt;<a href="ident?i=node">node</a>, <i>"port-number"</i>, <a href="ident?i=NULL">NULL</a>);
<a name=L598 href="source/linux/drivers/serial/uartlite.c#L598">598</a> 
<a name=L599 href="source/linux/drivers/serial/uartlite.c#L599">599</a>         return <a href="ident?i=ulite_assign">ulite_assign</a>(&amp;<a href="ident?i=op">op</a>-&gt;<a href="ident?i=dev">dev</a>, <a href="ident?i=id">id</a> ? *<a href="ident?i=id">id</a> : -1, <a href="ident?i=res">res</a>.<a href="ident?i=start">start</a>+3, <a href="ident?i=irq">irq</a>);
<a name=L600 href="source/linux/drivers/serial/uartlite.c#L600">600</a> }
<a name=L601 href="source/linux/drivers/serial/uartlite.c#L601">601</a> 
<a name=L602 href="source/linux/drivers/serial/uartlite.c#L602">602</a> static int <a href="ident?i=__devexit">__devexit</a> <a href="ident?i=ulite_of_remove">ulite_of_remove</a>(struct <a href="ident?i=of_device">of_device</a> *<a href="ident?i=op">op</a>)
<a name=L603 href="source/linux/drivers/serial/uartlite.c#L603">603</a> {
<a name=L604 href="source/linux/drivers/serial/uartlite.c#L604">604</a>         return <a href="ident?i=ulite_release">ulite_release</a>(&amp;<a href="ident?i=op">op</a>-&gt;<a href="ident?i=dev">dev</a>);
<a name=L605 href="source/linux/drivers/serial/uartlite.c#L605">605</a> }
<a name=L606 href="source/linux/drivers/serial/uartlite.c#L606">606</a> 
<a name=L607 href="source/linux/drivers/serial/uartlite.c#L607">607</a> static struct <a href="ident?i=of_platform_driver">of_platform_driver</a> <a href="ident?i=ulite_of_driver">ulite_of_driver</a> = {
<a name=L608 href="source/linux/drivers/serial/uartlite.c#L608">608</a>         .owner = <a href="ident?i=THIS_MODULE">THIS_MODULE</a>,
<a name=L609 href="source/linux/drivers/serial/uartlite.c#L609">609</a>         .<a href="ident?i=name">name</a> = <i>"uartlite"</i>,
<a name=L610 href="source/linux/drivers/serial/uartlite.c#L610">610</a>         .match_table = ulite_of_match,
<a name=L611 href="source/linux/drivers/serial/uartlite.c#L611">611</a>         .<a href="ident?i=probe">probe</a> = <a href="ident?i=ulite_of_probe">ulite_of_probe</a>,
<a name=L612 href="source/linux/drivers/serial/uartlite.c#L612">612</a>         .<a href="ident?i=remove">remove</a> = <a href="ident?i=__devexit_p">__devexit_p</a>(<a href="ident?i=ulite_of_remove">ulite_of_remove</a>),
<a name=L613 href="source/linux/drivers/serial/uartlite.c#L613">613</a>         .<a href="ident?i=driver">driver</a> = {
<a name=L614 href="source/linux/drivers/serial/uartlite.c#L614">614</a>                 .<a href="ident?i=name">name</a> = <i>"uartlite"</i>,
<a name=L615 href="source/linux/drivers/serial/uartlite.c#L615">615</a>         },
<a name=L616 href="source/linux/drivers/serial/uartlite.c#L616">616</a> };
<a name=L617 href="source/linux/drivers/serial/uartlite.c#L617">617</a> 
<a name=L618 href="source/linux/drivers/serial/uartlite.c#L618">618</a> <b><i>/* Registration helpers to keep the number of #ifdefs to a minimum */</i></b>
<a name=L619 href="source/linux/drivers/serial/uartlite.c#L619">619</a> static inline int <a href="ident?i=__init">__init</a> <a href="ident?i=ulite_of_register">ulite_of_register</a>(void)
<a name=L620 href="source/linux/drivers/serial/uartlite.c#L620">620</a> {
<a name=L621 href="source/linux/drivers/serial/uartlite.c#L621">621</a>         <a href="ident?i=pr_debug">pr_debug</a>(<i>"uartlite: calling of_register_platform_driver()\n"</i>);
<a name=L622 href="source/linux/drivers/serial/uartlite.c#L622">622</a>         return <a href="ident?i=of_register_platform_driver">of_register_platform_driver</a>(&amp;<a href="ident?i=ulite_of_driver">ulite_of_driver</a>);
<a name=L623 href="source/linux/drivers/serial/uartlite.c#L623">623</a> }
<a name=L624 href="source/linux/drivers/serial/uartlite.c#L624">624</a> 
<a name=L625 href="source/linux/drivers/serial/uartlite.c#L625">625</a> static inline void <a href="ident?i=__exit">__exit</a> <a href="ident?i=ulite_of_unregister">ulite_of_unregister</a>(void)
<a name=L626 href="source/linux/drivers/serial/uartlite.c#L626">626</a> {
<a name=L627 href="source/linux/drivers/serial/uartlite.c#L627">627</a>         <a href="ident?i=of_unregister_platform_driver">of_unregister_platform_driver</a>(&amp;<a href="ident?i=ulite_of_driver">ulite_of_driver</a>);
<a name=L628 href="source/linux/drivers/serial/uartlite.c#L628">628</a> }
<a name=L629 href="source/linux/drivers/serial/uartlite.c#L629">629</a> #else <b><i>/* CONFIG_OF */</i></b>
<a name=L630 href="source/linux/drivers/serial/uartlite.c#L630">630</a> <b><i>/* CONFIG_OF not enabled; do nothing helpers */</i></b>
<a name=L631 href="source/linux/drivers/serial/uartlite.c#L631">631</a> static inline int <a href="ident?i=__init">__init</a> <a href="ident?i=ulite_of_register">ulite_of_register</a>(void) { return 0; }
<a name=L632 href="source/linux/drivers/serial/uartlite.c#L632">632</a> static inline void <a href="ident?i=__exit">__exit</a> <a href="ident?i=ulite_of_unregister">ulite_of_unregister</a>(void) { }
<a name=L633 href="source/linux/drivers/serial/uartlite.c#L633">633</a> #endif <b><i>/* CONFIG_OF */</i></b>
<a name=L634 href="source/linux/drivers/serial/uartlite.c#L634">634</a> 
<a name=L635 href="source/linux/drivers/serial/uartlite.c#L635">635</a> <b><i>/* ---------------------------------------------------------------------</i></b>
<a name=L636 href="source/linux/drivers/serial/uartlite.c#L636">636</a> <b><i> * Module setup/teardown</i></b>
<a name=L637 href="source/linux/drivers/serial/uartlite.c#L637">637</a> <b><i> */</i></b>
<a name=L638 href="source/linux/drivers/serial/uartlite.c#L638">638</a> 
<a name=L639 href="source/linux/drivers/serial/uartlite.c#L639">639</a> int <a href="ident?i=__init">__init</a> <a href="ident?i=ulite_init">ulite_init</a>(void)
<a name=L640 href="source/linux/drivers/serial/uartlite.c#L640">640</a> {
<a name=L641 href="source/linux/drivers/serial/uartlite.c#L641">641</a>         int <a href="ident?i=ret">ret</a>;
<a name=L642 href="source/linux/drivers/serial/uartlite.c#L642">642</a> 
<a name=L643 href="source/linux/drivers/serial/uartlite.c#L643">643</a>         <a href="ident?i=pr_debug">pr_debug</a>(<i>"uartlite: calling uart_register_driver()\n"</i>);
<a name=L644 href="source/linux/drivers/serial/uartlite.c#L644">644</a>         <a href="ident?i=ret">ret</a> = <a href="ident?i=uart_register_driver">uart_register_driver</a>(&amp;<a href="ident?i=ulite_uart_driver">ulite_uart_driver</a>);
<a name=L645 href="source/linux/drivers/serial/uartlite.c#L645">645</a>         if (<a href="ident?i=ret">ret</a>)
<a name=L646 href="source/linux/drivers/serial/uartlite.c#L646">646</a>                 goto err_uart;
<a name=L647 href="source/linux/drivers/serial/uartlite.c#L647">647</a> 
<a name=L648 href="source/linux/drivers/serial/uartlite.c#L648">648</a>         <a href="ident?i=ret">ret</a> = <a href="ident?i=ulite_of_register">ulite_of_register</a>();
<a name=L649 href="source/linux/drivers/serial/uartlite.c#L649">649</a>         if (<a href="ident?i=ret">ret</a>)
<a name=L650 href="source/linux/drivers/serial/uartlite.c#L650">650</a>                 goto err_of;
<a name=L651 href="source/linux/drivers/serial/uartlite.c#L651">651</a> 
<a name=L652 href="source/linux/drivers/serial/uartlite.c#L652">652</a>         <a href="ident?i=pr_debug">pr_debug</a>(<i>"uartlite: calling platform_driver_register()\n"</i>);
<a name=L653 href="source/linux/drivers/serial/uartlite.c#L653">653</a>         <a href="ident?i=ret">ret</a> = <a href="ident?i=platform_driver_register">platform_driver_register</a>(&amp;<a href="ident?i=ulite_platform_driver">ulite_platform_driver</a>);
<a name=L654 href="source/linux/drivers/serial/uartlite.c#L654">654</a>         if (<a href="ident?i=ret">ret</a>)
<a name=L655 href="source/linux/drivers/serial/uartlite.c#L655">655</a>                 goto err_plat;
<a name=L656 href="source/linux/drivers/serial/uartlite.c#L656">656</a> 
<a name=L657 href="source/linux/drivers/serial/uartlite.c#L657">657</a>         return 0;
<a name=L658 href="source/linux/drivers/serial/uartlite.c#L658">658</a> 
<a name=L659 href="source/linux/drivers/serial/uartlite.c#L659">659</a> err_plat:
<a name=L660 href="source/linux/drivers/serial/uartlite.c#L660">660</a>         <a href="ident?i=ulite_of_unregister">ulite_of_unregister</a>();
<a name=L661 href="source/linux/drivers/serial/uartlite.c#L661">661</a> err_of:
<a name=L662 href="source/linux/drivers/serial/uartlite.c#L662">662</a>         <a href="ident?i=uart_unregister_driver">uart_unregister_driver</a>(&amp;<a href="ident?i=ulite_uart_driver">ulite_uart_driver</a>);
<a name=L663 href="source/linux/drivers/serial/uartlite.c#L663">663</a> err_uart:
<a name=L664 href="source/linux/drivers/serial/uartlite.c#L664">664</a>         <a href="ident?i=printk">printk</a>(<a href="ident?i=KERN_ERR">KERN_ERR</a> <i>"registering uartlite driver failed: err=%i"</i>, <a href="ident?i=ret">ret</a>);
<a name=L665 href="source/linux/drivers/serial/uartlite.c#L665">665</a>         return <a href="ident?i=ret">ret</a>;
<a name=L666 href="source/linux/drivers/serial/uartlite.c#L666">666</a> }
<a name=L667 href="source/linux/drivers/serial/uartlite.c#L667">667</a> 
<a name=L668 href="source/linux/drivers/serial/uartlite.c#L668">668</a> void <a href="ident?i=__exit">__exit</a> <a href="ident?i=ulite_exit">ulite_exit</a>(void)
<a name=L669 href="source/linux/drivers/serial/uartlite.c#L669">669</a> {
<a name=L670 href="source/linux/drivers/serial/uartlite.c#L670">670</a>         <a href="ident?i=platform_driver_unregister">platform_driver_unregister</a>(&amp;<a href="ident?i=ulite_platform_driver">ulite_platform_driver</a>);
<a name=L671 href="source/linux/drivers/serial/uartlite.c#L671">671</a>         <a href="ident?i=ulite_of_unregister">ulite_of_unregister</a>();
<a name=L672 href="source/linux/drivers/serial/uartlite.c#L672">672</a>         <a href="ident?i=uart_unregister_driver">uart_unregister_driver</a>(&amp;<a href="ident?i=ulite_uart_driver">ulite_uart_driver</a>);
<a name=L673 href="source/linux/drivers/serial/uartlite.c#L673">673</a> }
<a name=L674 href="source/linux/drivers/serial/uartlite.c#L674">674</a> 
<a name=L675 href="source/linux/drivers/serial/uartlite.c#L675">675</a> <a href="ident?i=module_init">module_init</a>(<a href="ident?i=ulite_init">ulite_init</a>);
<a name=L676 href="source/linux/drivers/serial/uartlite.c#L676">676</a> <a href="ident?i=module_exit">module_exit</a>(<a href="ident?i=ulite_exit">ulite_exit</a>);
<a name=L677 href="source/linux/drivers/serial/uartlite.c#L677">677</a> 
<a name=L678 href="source/linux/drivers/serial/uartlite.c#L678">678</a> <a href="ident?i=MODULE_AUTHOR">MODULE_AUTHOR</a>(<i>"Peter Korsgaard &lt;jacmet@sunsite.dk&gt;"</i>);
<a name=L679 href="source/linux/drivers/serial/uartlite.c#L679">679</a> <a href="ident?i=MODULE_DESCRIPTION">MODULE_DESCRIPTION</a>(<i>"Xilinx uartlite serial driver"</i>);
<a name=L680 href="source/linux/drivers/serial/uartlite.c#L680">680</a> <a href="ident?i=MODULE_LICENSE">MODULE_LICENSE</a>(<i>"GPL"</i>);
<a name=L681 href="source/linux/drivers/serial/uartlite.c#L681">681</a> </pre>
<table width="100%"><tr><td bgcolor="cccc99">&nbsp;
<font size=1>
This page was automatically generated by the 
<a href="http://lxr.linux.no/">LXR engine</a>.
</font></td></tr></table>
</html>
