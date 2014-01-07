// dhtml_lib loader V1.0
// detection du navigateur et chargement de la bonne librairie
// sinon un message d'erreur est envoyé
var agent = navigator.userAgent.toLowerCase();
var major = parseInt(navigator.appVersion);
var minor = parseFloat(navigator.appVersion);
var ie = ((agent.indexOf("msie") != -1) || (agent.indexOf("opera") != -1));
var ns = ((agent.indexOf('mozilla')!=-1) && (agent.indexOf('spoofer')==-1) && (agent.indexOf('compatible') == -1));
var ns4 = (ns && (major >= 4 && major<5));
var ns6 = (ns && (major >= 5));
var ie5 = (ie && (major >= 4));
if (ie5) {document.write("<script language='javascript' src='libdhtml_ie_lite.js'></script>")}
else if (ns6) {document.write("<script language='javascript' src='libdhtml_ns6_lite.js'></script>")}
else if (ns4) {document.write("<script language='javascript' src='libdhtml_ns4_lite.js'></script>")}
else {alert("Votre navigateur n'est pas compatible avec la DHTML lib V2.3 (IE4.x+/NS4.x+)")}
