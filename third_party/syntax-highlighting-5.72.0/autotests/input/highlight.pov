//BEGIN TEST
// test scene for POV-Ray syntax highlighting

/* test comment */

/* nested /* comments */ do not work */

#version 3.5;
//END
global_settings { assumed_gamma 1.0 }

// ----------------------------------------

camera {
  location  <5.0, -12.0, 2.0>
  up z sky z
  look_at   <0.0, 0.0, 0.5> 
  angle 40
}

sky_sphere {
  pigment {
    gradient z
    color_map {
      [0.0 rgb <0.6,0.7,1.0>]
      [0.2 rgb <0.2,0.3,0.9>]
    }
  }
}

light_source {
  <3, 1, 2>*1000
  color rgb <2.2, 1.8, 1.5>
}   

// ----------------------------------------

#declare TEST=0;

#ifdef (TEST)
  plane {
    z, 0
    texture {
      pigment {
        checker
        color rgb 1, color rgb 0
      }
    }
  }
#end

#macro Sphere(Pos, Radius)
  sphere {
    <Pos.x, Pos.y, Radius*1.3>, Radius
    texture {
      pigment { color rgb 1 }
      finish{
        diffuse 0.3
        ambient 0.0
        specular 0.6
        reflection 0.8
      }
    }
  }
#end

#local Cnt=0;
#local Seed=seed(0);

#while (Cnt<10000)
  Sphere(
    -100+<rand(Seed), rand(Seed)>*200, 
    0.3+pow(rand(Seed),2)*0.7
  )  
  #local Cnt=Cnt+1;
#end