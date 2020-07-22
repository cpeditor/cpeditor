/* kate: hl CartoCSS
   This file contains some content coming from
   https://github.com/gravitystorm/openstreetmap-carto
   with CC0 license. This file is just for testing
   katepart highlighting engine.
   */

#world {
// this syntax
polygon-opacity: 50%;

// is equivalent to
polygon-opacity: 0.5;
}

@admin-boundaries: #ac46ac;

/* For performance reasons, the admin border layers are split into three groups
for low, middle and high zoom levels.
For each zoomlevel, all borders come from a single attachment, to handle
overlapping borders correctly.
*/

#admin-low-zoom[zoom < 11],  // test
#admin-mid-zoom[zoom >= 11][zoom < 13],
#admin-high-zoom[zoom >= 13] {
  [admin_level = '2'],
  [admin_level = '3'] {
    [zoom >= 4] {
      background/line-color: white;
      background/line-width: 0.6;
      line-color: @admin-boundaries;
      line-width: 0.6;
    }
    [zoom >= 7] {
      background/line-width: 2;
      line-width: 2;
    }
    [zoom >= 10] {
      [admin_level = '2'] {
        background/line-width: 6;
        line-width: 6;
      }
      [admin_level = '3'] {
        background/line-width: 5;
        line-width: 5;
        line-dasharray: 4,2;
        line-clip: false;
      }
    }
  }
  [admin_level = '4'] {
    [zoom >= 4] {
      background/line-color: white;
      background/line-width: 0.6;
      line-color: @admin-boundaries;
      line-width: 0.6;
      line-dasharray: 4,3;
      line-clip: false;
    }
    [zoom >= 7] {
      background/line-width: 1;
      line-width: 1;
    }
    [zoom >= 11] {
      background/line-width: 3;
      line-width: 3;
    }
  }
  /*
  The following code prevents admin boundaries from being rendered on top of
  each other. Comp-op works on the entire attachment, not on the individual
  border. Therefore, this code generates an attachment containing a set of
  @admin-boundaries/white dashed lines (of which only the top one is visible),
  and with `comp-op: darken` the white part is ignored, while the
  @admin-boundaries colored part is rendered (as long as the background is not
  darker than @admin-boundaries).
  The SQL has `ORDER BY admin_level`, so the boundary with the lowest
  admin_level is rendered on top, and therefore the only visible boundary.
  */
  opacity: 0.4;
  comp-op: darken;
}

#admin-mid-zoom[zoom >= 11][zoom < 13],
#admin-high-zoom[zoom >= 13] {
  [admin_level = '5'][zoom >= 11] {
    background/line-color: white;
    background/line-width: 2;
    line-color: @admin-boundaries;
    line-width: 2;
    line-dasharray: 6,3,2,3,2,3;
    line-clip: false;
  }
  [admin_level = '6'][zoom >= 11] {
    background/line-color: white;
    background/line-width: 2;
    line-color: @admin-boundaries;
    line-width: 2;
    line-dasharray: 6,3,2,3;
    line-clip: false;
  }
  [admin_level = '7'],
  [admin_level = '8'] {
    [zoom >= 12] {
      background/line-color: white;
      background/line-width: 1.5;
      line-color: @admin-boundaries;
      line-width: 1.5;
      line-dasharray: 5,2;
      line-clip: false;
    }
  }
  opacity: 0.5;
  comp-op: darken;
}

#admin-high-zoom[zoom >= 13] {
  [admin_level = '9'],
  [admin_level = '10'] {
    [zoom >= 13] {
      background/line-color: white;
      background/line-width: 2;
      line-color: @admin-boundaries;
      line-width: 2;
      line-dasharray: 2,3;
      line-clip: false;
    }
  }
  opacity: 0.5;
  comp-op: darken;
}



#nature-reserve-boundaries {
  [way_pixels > 100][zoom >= 7] {
    [zoom < 10] {
      ::fill {
        opacity: 0.05;
        polygon-fill: green;
      }
    }
    a/line-width: 1;
    a/line-offset: -0.5;
    a/line-color: green;
    a/line-opacity: 0.15;
    a/line-join: round;
    a/line-cap: round;
    b/line-width: 2;
    b/line-offset: -1;
    b/line-color: green;
    b/line-opacity: 0.15;
    b/line-join: round;
    b/line-cap: round;
    [zoom >= 10] {
      a/line-width: 2;
      a/line-offset: -1;
      b/line-width: 4;
      b/line-offset: -2;
    }
    [zoom >= 14] {
      b/line-width: 6;
      b/line-offset: -3;
    }
  }
}

#building-text {
[zoom >= 14][way_pixels > 3000],
[zoom >= 17] {
text-name: "[name]";
text-size: 11;
text-fill: #444;
text-face-name: @book-fonts;
text-halo-radius: 1;
text-wrap-width: 20;
text-halo-fill: rgba(255,255,255,0.5);
text-placement: interior;
}
}

@marina-text: #576ddf; // also swimming_pool
@landcover-face-name: @oblique-fonts;
@standard-wrap-width: 30;

.points {
  [feature = 'tourism_alpine_hut'][zoom >= 13] {
    point-file: url('symbols/alpinehut.p.16.png');
    point-placement: interior;
  }
  }

  [feature = 'highway_bus_stop'] {
    [zoom >= 16] {
      marker-file: url('symbols/square.svg');
      marker-fill: @transportation-icon;
      marker-placement: interior;
      marker-width: 6;
    }
    [zoom >= 17] {
      marker-file: url('symbols/bus_stop.p.12.png');
      marker-width: 12;
    }
  }

[feature = 'highway_primary'] {
[zoom >= 7][zoom < 12] {
line-width: 0.5;
line-color: @primary-fill;
[zoom >= 9] { line-width: 1.2; }
[zoom >= 10] { line-width: 2; }
[zoom >= 11] { line-width: .5; }
}
}

#world {
  // this syntax
  polygon-opacity: 50%; // the parameter “polygon-opacity” gets the value 0.5 (expressed in %)
  // is equivalent to
  polygon-opacity: 0.5;
}

/*
Variables behave similar to C macros
*/
@lsdjkf: @sdlfkj; // this variable gets defined by the value of another variable
@admin-boundaries: #ac46ac; // this variable gets defined by a color value
@way_pixels: "([way_area]*pow(4,@zoom)/24505740000)"; // A rather complex variable. [way_area] marks a data field. @zoom is a special runtime value (inspite of the @ it has nothing to do with ordinary variables)

/* This is
a multiline comment.
*/

// A single line comment
#admin-low-zoom[zoom < 11],  // A single line comment
#admin-mid-zoom[zoom >= 11][zoom < 13] {
  [admin_level = '2'], // Within filters, data fields like “admin_level” are referended without extra [] brackets.
  [admin_level = '3'] { // These data fields are rendered by default in dark blue, while the data field within expression strings are rendered by default in light blue.
    [zoom >= 4] {
      background/line-color: white; // symbolizer named “background”
      background/line-width: 0.6; // symbolizer named “background”
      line-color: @admin-boundaries; // default symbolizer (without name)
      name: [test]; // simplified reference to the data field “test”
      name: "[test]"; // another reference to the data field “test”, this time within an expression string (by default orange)
      name: "([way_area]*pow(4,@zoom)/24505740000)"; // a rather complex expression string that will do some math
      name: "'Value: '[test]"; // A verbatim string (by default red) as part of an expression string
      name: '"Value: "[test]'; // " and ' are interchangable. The outer is always the expression string and the inner the verbatim string.
    }
  }
  [admin_level = '4'] { // The string '4' is red because at this position it will be interpreted as a verbatim string, not as an expression string.
    [zoom >= 4] {
      line-dasharray: 4,3; // parameter “line-dasharray” gets as value a list of two integers
      line-clip: false;  // parameter “line-clip” gets as value a boolean
    }
  }
  comp-op: darken; // parameter “comp-op” gets as value “darken”.
}

.nature-reserve-boundaries { // .nature-reserve-boudaries references a class, which is similar to a layer like #building-text, so both are rendered the same way
  [way_pixels > 100][zoom >= 7] { // Here zoom is a keyword with results in a special filter. However, all other values are interpreted as data fields.
    [zoom < 10] {
      ::fill { // The :: syntax defined “attachments” (a sort of sub-layer within normal layers). So “fill” is rendered by default in the same style like layer names and class names (but this can be customized).
        opacity: 0.05;
        // various styles to define colors (all except the color function are rendered the same way):
        polygon-fill: white;
        polygon-fill: #ffffff;
        polygon-fill: #fff;
        polygon-fill: rgba(255,255,255,1); // define a color by a speciel function
        polygon-fill: #ffff; // invalid color value
        text-placement: interior; // all unknown values are hightlighted as named values.
      }
    }
  }
}
