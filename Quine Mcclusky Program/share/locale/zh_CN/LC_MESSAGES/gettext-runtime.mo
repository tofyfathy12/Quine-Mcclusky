Þ    &      L  5   |      P  8   Q  B     A   Í  6     H   F  I     F   Ù  9      7   Z  6     M   É  L     O   d  H   ´  {   ý     y  Ù     e   `  :   Æ    	  Þ  
    ú          ¢  <   ³  1   ð  &   "     I  "   X  9   {  I   µ     ÿ          ­     Æ     Ø     ç  Ó  ú  8   Î  :     :   B  9   }  I   ·  I     D   K  7     7   È  5      D   6  >   {  V   º  I     \   [     ¸  ä   Å  R   ª  .   ý  
  ,  ¶  7  O  î     >     R  ¥   c     	  .   &     U  #   d  ;     K   Ä          ¡     ³     Ë     Û     è              !          #                                                      %                                        
                  $      	   &                 "                  -E                        (ignored for compatibility)
   -V, --version               output version information and exit
   -V, --version             display version information and exit
   -c, --context=CONTEXT     specify context for MSGID
   -d, --domain=TEXTDOMAIN   retrieve translated message from TEXTDOMAIN
   -d, --domain=TEXTDOMAIN   retrieve translated messages from TEXTDOMAIN
   -e                        enable expansion of some escape sequences
   -h, --help                  display this help and exit
   -h, --help                display this help and exit
   -n                        suppress trailing newline
   -v, --variables             output the variables occurring in SHELL-FORMAT
   COUNT                     choose singular/plural form based on this value
   MSGID MSGID-PLURAL        translate MSGID (singular) / MSGID-PLURAL (plural)
   [TEXTDOMAIN]              retrieve translated message from TEXTDOMAIN
   [TEXTDOMAIN] MSGID        retrieve translated message corresponding
                            to MSGID from TEXTDOMAIN
 Bruno Haible Copyright (C) %s Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <%s>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
 Display native language translation of a textual message whose grammatical
form depends on a number.
 Display native language translation of a textual message.
 If the TEXTDOMAIN parameter is not given, the domain is determined from the
environment variable TEXTDOMAIN.  If the message catalog is not found in the
regular directory, another location can be specified with the environment
variable TEXTDOMAINDIR.
Standard search directory: %s
 If the TEXTDOMAIN parameter is not given, the domain is determined from the
environment variable TEXTDOMAIN.  If the message catalog is not found in the
regular directory, another location can be specified with the environment
variable TEXTDOMAINDIR.
When used with the -s option the program behaves like the 'echo' command.
But it does not simply copy its arguments to stdout.  Instead those messages
found in the selected catalog are translated.
Standard search directory: %s
 In normal operation mode, standard input is copied to standard output,
with references to environment variables of the form $VARIABLE or ${VARIABLE}
being replaced with the corresponding values.  If a SHELL-FORMAT is given,
only those environment variables that are referenced in SHELL-FORMAT are
substituted; otherwise all environment variables references occurring in
standard input are substituted.
 Informative output:
 Operation mode:
 Report bugs in the bug tracker at <%s>
or by email to <%s>.
 Substitutes the values of environment variables.
 Try '%s --help' for more information.
 Ulrich Drepper Usage: %s [OPTION] [SHELL-FORMAT]
 Usage: %s [OPTION] [TEXTDOMAIN] MSGID MSGID-PLURAL COUNT
 Usage: %s [OPTION] [[TEXTDOMAIN] MSGID]
or:    %s [OPTION] -s [MSGID]...
 When --variables is used, standard input is ignored, and the output consists
of the environment variables that are referenced in SHELL-FORMAT, one per line.
 Written by %s.
 error while reading "%s" missing arguments standard input too many arguments Project-Id-Version: gettext-runtime 0.20.2
Report-Msgid-Bugs-To: bug-gettext@gnu.org
PO-Revision-Date: 2020-04-22 12:08-0400
Last-Translator: Boyuan Yang <073plan@gmail.com>
Language-Team: Chinese (simplified) <i18n-zh@googlegroups.com>
Language: zh_CN
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
Plural-Forms: nplurals=1; plural=0;
X-Bugs: Report translation errors to the Language-Team address.
X-Generator: Poedit 2.3
   -E                        ï¼ä¸ºå¼å®¹æ§èå¿½ç¥ï¼
   -V, --version               è¾åºçæ¬ä¿¡æ¯å¹¶éåº
   -V, --version               è¾åºçæ¬ä¿¡æ¯å¹¶éåº
   -c, --context=ä¸ä¸æ      æå® MSGID çä¸ä¸æ
   -d, --domain=ææ¬å       ä»æå®<ææ¬å>è·åå·²ç¿»è¯æ¶æ¯
   -d, --domain=ææ¬å       ä»æå®<ææ¬å>è·åå·²ç¿»è¯æ¶æ¯
   -e                        å¯ç¨å¯¹æäºè½¬ä¹åºåè¿è¡å±å¼
   -h, --help                  æ¾ç¤ºæ­¤å¸®å©å¹¶éåº
   -h, --help                  æ¾ç¤ºæ­¤å¸®å©å¹¶éåº
   -n                        å»é¤æ«å°¾çæ¢è¡ç¬¦
   -v, --variables             è¾åº SHELLæ ¼å¼ ä¸­åºç°çåé
   COUNT                     åºäºæ­¤å¼éæ©åå¤æ°å½¢å¼
   MSGID MSGID-PLURAL        ç¿»è¯ MSGID (åæ°å½¢å¼) / MSGID-PLURAL (å¤æ°å½¢å¼)
   [ææ¬å]                  ä»æå®<ææ¬å>è·åå·²ç¿»è¯æ¶æ¯
   [ææ¬å] MSGID            ä»æå®<ææ¬å>è·åå¯¹åºäº MSGID çå·²ç¿»è¯æ¶æ¯
 Bruno Haible çæææ (C) %s Free Software Foundation, Inc.
ææåè®® GPLv3+: GNU GPL çæ¬ 3 æä¹åçæ¬ <%s>
è¿æ¯èªç±è½¯ä»¶: æ¨æä¿®æ¹åååå¸çèªç±ã
æ¬è½¯ä»¶å¨æ³å¾åè®¸çèå´åä¸æä¾ä»»ä½æä¿ã
 æ¾ç¤ºæåææ¶æ¯çæ¬å°è¯­è¨ç¿»è¯ï¼èç¿»è¯çè¯­æ³åæ°å­æå³ã
 æ¾ç¤ºæåææ¶æ¯çæ¬å°è¯­è¨ç¿»è¯ã
 åå¦æ²¡ææå®<ææ¬å>åæ°ï¼ç¨åºä¼æ ¹æ® TEXTDOMAIN ç¯å¢åéæ¥è®¾å®ææ¬åã
åå¦æ¶æ¯ä¸è§è¡¨æä»¶ä¸å¨æ­£å¸¸çç®å½ä¸ï¼å¯ä»¥ç¨ç¯å¢åé TEXTDOMAINDIR æå®
æ¶æ¯ä¸è§è¡¨æä»¶æå¨çä½ç½®ã
æ åçæç´¢ç®å½ä¸ºï¼%s
 åå¦æ²¡ææå®<ææ¬å>åæ°ï¼ç¨åºä¼æ ¹æ® TEXTDOMAIN ç¯å¢åéæ¥è®¾å®<ææ¬å>ã
åå¦æ¶æ¯ä¸è§è¡¨æä»¶ä¸å¨æ­£å¸¸çç®å½ä¸ï¼å¯ä»¥ç¨ç¯å¢åé TEXTDOMAINDIR æå®
æ¶æ¯ä¸è§è¡¨æä»¶æå¨çä½ç½®ã
ä½¿ç¨éé¡¹â-sâæ¶æ­¤ç¨åºåâechoâå½ä»¤ç±»ä¼¼ãä½å®ä¸æ¯ç´æ¥æåæ°å¤å¶è³æ å
è¾åºï¼èæ¯å¯»æ¾æ¶æ¯ä¸è§è¡¨æä»¶éç¿»è¯è¿çæ¶æ¯ã
æ åçæç´¢ç®å½ä¸ºï¼%s
 å¨æ®éæä½æ¨¡å¼ä¸ï¼ä¼å°æ åè¾å¥å¤å¶å°æ åè¾åºä¸­ï¼èä»¥ $VARIABLE æ ${VARIABLE}
æ ¼å¼ç»åºçç¯å¢åéå¼ç¨ä¼è¢«æ¿æ¢æç¸åºçå¼ãå¦æç»åºäº SHELLæ ¼å¼ï¼ååªæå¨è¯¥æ ¼å¼
ä¸­å¼ç¨çç¯å¢åéä¼è¢«æ¿æ¢ï¼å¦åï¼æ åè¾å¥ä¸­åºç°çå¨é¨ç¯å¢åéå¼ç¨é½å°è¢«æ¿æ¢ã
 ä¿¡æ¯æ§è¾åºï¼
 æä½æ¨¡å¼ï¼
 è¯·å°è½¯ä»¶éè¯¯æäº¤è³ bug è·è¸ªç³»ç» <%s>
æåéçµå­é®ä»¶å° <%s>ã
è¯·å°ç¿»è¯éè¯¯æ¥åè³ç®ä½ä¸­æé®ä»¶åè¡¨ <i18n-zh@googlegroups.com>ã
 æ¿æ¢ç¯å¢åéçå¼ã
 è¯è¯â%s --helpâæ¥è·åæ´å¤ä¿¡æ¯ã
 Ulrich Drepper ç¨æ³ï¼%s [éé¡¹] [SHELLæ ¼å¼]
 ç¨æ³ï¼%s [éé¡¹] [ææ¬å] MSGID å¤æ°-MSGID æ°å­
 ç¨æ³ï¼%s [éé¡¹] [[ææ¬å] MSGID]
æï¼  %s [éé¡¹] -s [MSGID]...
 è¥ä½¿ç¨äº --variablesï¼åä¼å¿½ç¥æ åè¾å¥ï¼èè¾åºç± SHELLæ ¼å¼ å¼ç¨çç¯å¢åéç»æï¼
æ¯è¡åºç°ä¸ä¸ªç¯å¢åéã
 ç± %s ç¼åã
 è¯»åâ%sâæ¶åºé ç¼ºå°äºåæ° æ åè¾å¥ åæ°è¿å¤ 