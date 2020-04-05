#!/usr/local/bin/tpicoc
{
  hexvalue(digit)::
    if (is_void(number(char(digit))),
       if (digit > ord("Z"),
          10 + digit - ord("a"),
          10 + digit - ord("A")),
       number(char(digit)));
        
  urldecode(encoded):: {
    decoded[size(encoded)]: ord(" ");
    eindex: dindex: 0;
    while ((eindex:=eindex+1) <= size(encoded),
       if (encoded[eindex] = ord("%"),
          decoded[dindex:=dindex+1] :=
             16 * hexvalue(encoded[eindex:=eindex+1])
                + hexvalue(encoded[eindex:=eindex+1]),
          if (encoded[eindex] = ord("+"),
             decoded[dindex:=dindex+1] := ord(" "),
             decoded[dindex:=dindex+1] := encoded[eindex])));
    count: 0;
    trim[dindex]: decoded[count:=count+1]
  };

  display("Content-Type: text/html", eoln, eoln);

  display("<h1>Pico demo</h1>", eoln);

  display("This <a href='tpico-demo.txt'>cgi script</a> as well as ");
  display("its user input is run by an unmodified but sandboxed pico 2.0 ");
  display("interpreter, i.e. with its load function disabled and its ");
  display("maximum runtime set to 10 seconds.", eoln);
  
  example:: "{"           + eoln +
            "  f(x):x*7;" + eoln +
            "  f(3*2)"    + eoln +
            "}";

  input::
    if (size(inputtab: urldecode(explode(accept()))) = 0,
       example,
       { count: 0;
         while (inputtab[count:=count+1] != ord("="), void);
         valuetab[size(inputtab)-count]: inputtab[count:=count+1];
         implode(valuetab)});

  display("<p><form method='post' action='tpico-demo.cgi'>");
  display("<textarea name='input' rows=16 cols=80>", input, "</textarea>");
  display("<p><input type='submit' value='evaluate'></form><p>", eoln);
  
  load(ignore)::
    display("load() has been disabled in this demo...", eoln);
  accept()::
    display("accept() does not do much in this demo...", eoln);

  display("<pre>", eval(read(input)), "</pre>")
}
