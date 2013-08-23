MayaShapeSample
===============

A simple example of how to add user-defined shapes to Maya using plugins.

Useage
======
Load the plugin into Maya
In the MEL parser, write:

    string $node = `createNode simpleShape`; // You'll see nothing.
    // Now add some CVs, one
    string $attr = $node + ".controlPoints[0]";
    setAttr $attr 2 2 2; 					// Now you'll have something on screen, in wireframe mode
    // or a bunch
    int $idx = 0;
    for ( $i=0; $i<100; $i++)
    {
       for ( $j=0; $j<100; $j++)
       {
           string $attr = $node + ".controlPoints[ " + $idx + "]";
           setAttr $attr $i $j 3;
           $idx++;
       }
    }

Note
====
I did not come up with the plugin, it is an interpretation/rewrite
of the apiSimpleShape plugin that ships with Maya 2013. I simply write it
up again manually in order to learn how to introduce shapes in Maya.