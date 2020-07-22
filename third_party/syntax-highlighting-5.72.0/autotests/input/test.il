// Test file for the Common Intermediate Language

/*
  multi line comment
*/

// examples from Wikipedia
.assembly Hello {}
.assembly extern mscorlib {}
.method static void Main()
{
     .entrypoint
     .maxstack 1
     ldstr "Hello, world!"
     call void [mscorlib]System.Console::WriteLine(string)
     ret
}

.method private hidebysig static void Main(string[] args) cil managed
{
    .entrypoint
    .maxstack  2
    .locals init (int32 V_0,
                  int32 V_1)

              ldc.i4.2
              stloc.0
              br.s       IL_001f
    IL_0004:  ldc.i4.2
              stloc.1
              br.s       IL_0011
    IL_0008:  ldloc.0
              ldloc.1
              rem
              brfalse.s  IL_001b
              ldloc.1
              ldc.i4.1
              add
              stloc.1
    IL_0011:  ldloc.1
              ldloc.0
              blt.s      IL_0008
              ldloc.0
              call       void [mscorlib]System.Console::WriteLine(int32)
    IL_001b:  ldloc.0
              ldc.i4.1
              add
              stloc.0
    IL_001f:  ldloc.0
              ldc.i4     0x3e8
              blt.s      IL_0004
              ret
}

.method assembly static void modopt([mscorlib]System.Runtime.CompilerServices.CallConvCdecl)
        test_pointer_operations(int32 param) cil managed
{
  .vtentry 1 : 1
  // Code size       44 (0x2c)
  .maxstack  2
  .locals ([0] int32* ptr,
           [1] valuetype A* V_1,
           [2] valuetype A* a,
           [3] int32 k)
// k = 0;
  IL_0000:  ldc.i4.0
  IL_0001:  stloc.3
// ptr = &k;
  IL_0002:  ldloca.s   k // load local's address instruction
  IL_0004:  stloc.0
// *ptr = 1;
  IL_0005:  ldloc.0
  IL_0006:  ldc.i4.1
  IL_0007:  stind.i4 // indirection instruction
// ptr = &param
  IL_0008:  ldarga.s   param // load parameter's address instruction
  IL_000a:  stloc.0
// *ptr = 2
  IL_000b:  ldloc.0
  IL_000c:  ldc.i4.2
  IL_000d:  stind.i4
// a = new A;
  IL_000e:  ldloca.s   a
  IL_0010:  call       valuetype A* modopt([mscorlib]System.Runtime.CompilerServices.CallConvThiscall) 'A.{ctor}'(valuetype A* modopt([mscorlib]System.Runtime.CompilerServices.IsConst) modopt([mscorlib]System.Runtime.CompilerServices.IsConst))
  IL_0015:  pop
// ptra = &a;
  IL_0016:  ldloca.s   a
  IL_0018:  stloc.1
// ptra->meth();
  IL_0019:  ldloc.1
  IL_001a:  dup
  IL_001b:  ldind.i4 // reading the VMT for virtual call
  IL_001c:  ldind.i4
  IL_001d:  calli      unmanaged stdcall void modopt([mscorlib]System.Runtime.CompilerServices.CallConvStdcall)(native int)
  IL_0022:  ret
} // end of method 'Global Functions'::test_pointer_operations
