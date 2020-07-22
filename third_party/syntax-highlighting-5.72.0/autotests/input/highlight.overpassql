/* Overpass samples taken from
   https://wiki.openstreetmap.org/wiki/Overpass_API/Overpass_QL
   https://wiki.openstreetmap.org/wiki/Overpass_API/Overpass_API_by_Example

*/

// single line comment with TODO alerts

/* multi-line comment with ### alerts */

[out:json][timeout:25];
// gather results
(
  // query part for: “amenity=post_box”
  node["amenity"="post_box"](52.0, 13.0, 54.0, 13.5);
  way["amenity"="post_box"]({{bbox}});
  relation["amenity"="post_box"]({{bbox}});
);
// print results
out body;
>;
out skel qt;


[out:csv(::type, "de:regionalschluessel", name,
         ::count, ::"count:nodes", ::"count:ways", ::"count:relations")];

//All areas with regional key (German: "Regionalschlüssel") starting with 057
area["de:regionalschluessel"~"^057"];

// Count the pharmacies in each area
foreach->.regio(
  // display details for the current area
  .regio out;

  // Collect all Nodes, Ways and Relations with amenity=pharmacy in the current area
  ( node(area.regio)[amenity=pharmacy];
    way(area.regio)[amenity=pharmacy];
    rel(area.regio)[amenity=pharmacy];);

// Count the elements in the current area Area
  out count;
);


[timeout:300]
[bbox:51.08282186160976,-12.8759765625,55.986091533808384,-1.86767578125]
[out:csv(::id, ::type, name, "name:en", "name:ga")];

( node[name]["name:en"]["name:ga"](if:(t["name"] != t["name:en"]) && (t["name"] != t["name:ga"]));
  way[name]["name:en"]["name:ga"] (if:(t["name"] != t["name:en"]) && (t["name"] != t["name:ga"]));
  rel[name]["name:en"]["name:ga"] (if:(t["name"] != t["name:en"]) && (t["name"] != t["name:ga"]));
);
out;


[out:csv(version,timestamp,changeset,count)];
timeline(relation,2632934);
for (t["created"])
{
  retro(_.val)
  {
    rel(2632934);
    make stat version=u(version()),timestamp=u(timestamp()),changeset=u(changeset()),count=u(count_members());
    out;
  }
}


[timeout:60];
area["name"="Mayenne"]->.boundaryarea;
way(area.boundaryarea)["building"="church"];
  foreach(
    node(w)->.d;                                     // determine all nodes of a way
    .n is_in->.a;
    area.a[name][boundary=administrative][admin_level~"^[2-8]$"] -> .a;
    out center;
    convert way ::=::,
              ::id = id(),
              is_in=a.set("{" + t["admin_level"] + ":" + t["name"] + "}");

    out;
);
