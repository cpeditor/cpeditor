# Test file for Julia

# Comment

#= Multi-line
   comment =#

#BEGIN

#END

"""
 Multi-line string
"""
```
 Multi-line command
```
raw"string\a\\a"

let z = zip(1:2, 3:4, 5:6)
    @test size(z) == (2,)
    @test collect(z) == [(1,3,5), (2,4,6)]
    @test eltype(z) == Tuple{Int,Int,Int}
end

@testset "generic conversion from Integer" begin
    x = rand(Int128)
    @test BigInt(x) % Int128 === x
    y = rand(UInt128)
    @test BigInt(y) % UInt128 === y
end

@testset "show" begin
    @test sprint(show, BitSet()) == "BitSet([])"
    @test sprint(show, BitSet([1,2,3])) == "BitSet([1, 2, 3])"
    show(IOBuffer(), BitSet())
end

cmd = `$(Base.julia_cmd()) --depwarn=error --startup-file=no boundscheck_exec.jl`
if !success(pipeline(cmd; stdout=stdout, stderr=stderr))
    error("boundscheck test failed, cmd : $cmd")
end

@test iterate(I, CartesianIndex(3, typemax(Int)))[1] == CartesianIndex(4,typemax(Int))
@test iterate(I, CartesianIndex(4, typemax(Int)))    === nothing
@test_throws MethodError write(IOBuffer(), ASCIIChar('x'))
@test_throws MethodError read(IOBuffer('x'), ASCIIChar)

let header = "julia [switches] -- [programfile] [args...]"
    @test startswith(read(`$exename -h`, String), header)
    @test startswith(read(`$exename --help`, String), header)
end

@test isequal(exp(complex( Inf, NaN)), complex(-Inf, NaN))
@test isequal(exp(complex( Inf, Inf)), complex(-Inf, NaN))

# Numbers
0b10
0o01_70
0x00000000000000001111__22_2233334444
-0x0002im
1.0e10
0.00025f0
-1.5__5f0
0xdeadbeefim
0x1.8p3
0x.4p-1

chars = ['0', '1', '2', '3', 'a', 'b', 'c', 'd', 'e', 'X', 'Y', 'Z',
         '𠀋', '𠀌', '𠀍', '🂠', '🂡', '🂢', '🂣', '🂮']

@test docstrings_equal(@doc(ModuleMacroDoc), doc"I am a module")
match(r"^\s*(?:#|$)", "# a comment")
