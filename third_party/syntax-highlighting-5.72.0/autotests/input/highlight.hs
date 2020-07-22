-- test file for Haskell syntax highlighting in KDE's Kate

-- The test file for literate Haskell can be easily created like this:
--  cat highlight.hs | sed -e "s|^|> |" -e "s|> -- ||" -e "s|^> $||" > highlight.lhs
-- You only have to manually edit the multi-line comment below.

-- this is a single-line comment

{- this is a multi-line comment

Things like "a string" or a 'c' character shouldn't be highlighted in here.

-- I could even start a new
-- one-line comment.

-}

import Prelude --

-- a data definition

data Tree a = Br (Tree a) (Tree a) | Leaf a | Nil deriving (Show, Eq)


-- function definition, "funnyfunction::", "Integer", "Int", "Bool" should be highlighted

funnyfunction::(Tree a)=>[a]->Integer->Int->Bool


-- strings and chars
-- first line of function definitions (type declaration) should be highlighted

strangefunction::Int->String
strangefunction 1 = "hello"
strangefunction 2 = "what's up"
strangefunction 3 = (strangefunction 1) ++ ", " ++ (strangefunction 2)
strangefunction 4 = 'a':'b':'c':'"':[] -- will return "abc"
strangefunction 5 = '\n':[]
strangefunction 6 = '\invalidhaskell':[]

-- function name including the single quote character
-- and infix operator (`div`)

justtesting'::Int->Int
justtesting' 2 = 2+1
justtesting' 9 = 7 `div` 2

-- same definition as above, slightly different function name and a couple more whitespaces

justtesting''	::	Int -> Int
justtesting''   2 = 3
justtesting''	9	=   3 + 9 - 9

-- the following lines are copied out of Haskell's "Prelude.hs"

infixl 7  *, /, `quot`, `rem`, `div`, `mod`, :%, %


-- everything highlighted except the "a"

class Bounded a where
    minBound, maxBound :: a

class (Num a, Ord a) => Real a where
    toRational     :: a -> Rational

-- finally, some keyword lists

-- keywords

case, class, data, deriving, do, else, family, forall, if, in, infixl, infixr,
instance, let, module, of, pattern, primitive,
then, type, where

-- infix operators

quot, rem, div, mod, elem, notElem, seq

-- this stuff is not handled yet

!!, %, &&, $!, $, *, **, -,., /=, <, <=, =<<, ==, >, >=, >>, >>=, ^, ^^, ++, ||

-- functions

abs, acos, acosh, all, and, any, appendFile,
approxRational, asTypeOf, asin, asinh, atan, atan2, atanh, basicIORun,
break, catch, ceiling, chr, compare, concat, concatMap, const, cos, cosh,
curry, cycle, decodeFloat, denominator, digitToInt, div, divMod, drop,
dropWhile, either, elem, encodeFloat, enumFrom, enumFromThen,
enumFromThenTo, enumFromTo, error, even, exp, exponent, fail, filter, flip,
floatDigits, floatRadix, floatRange, floor, fmap, foldl, foldl1, foldr,
foldr1, fromDouble, fromEnum, fromInt, fromInteger, fromIntegral,
fromRational, fst, gcd, getChar, getContents, getLine, head, id, inRange,
index, init, intToDigit, interact, ioError, isAlpha, isAlphaNum, isAscii,
isControl, isDenormalized, isDigit, isHexDigit, isIEEE, isInfinite, isLower,
isNaN, isNegativeZero, isOctDigit, isPrint, isSpace, isUpper, iterate, last,
lcm, length, lex, lexDigits, lexLitChar, lines, log, logBase, lookup, map,
mapM, mapM_, max, maxBound, maximum, maybe, min, minBound, minimum, mod,
negate, not, notElem, null, numerator, odd, or, ord, otherwise, pi, pred,
primExitWith, print, product, properFraction, putChar, putStr, putStrLn,
quot, quotRem, range, rangeSize, read, readDec, readFile, readFloat,
readHex, readIO, readInt, readList, readLitChar, readLn, readOct, readParen,
readSigned, reads, readsPrec, realToFrac, recip, rem, repeat, replicate,
return, reverse, round, scaleFloat, scanl, scanl1, scanr, scanr1, seq,
sequence, sequence_, show, showChar, showInt, showList, showLitChar,
showParen, showSigned, showString, shows, showsPrec, significand, signum,
sin, sinh, snd, span, splitAt, sqrt, subtract, succ, sum, tail, take,
either, elem, encodeFloat, enumFrom, enumFromThen, enumFromThenTo,
enumFromTo, error, even, exp, exponent, fail, filter, flip, floatDigits,
floatRadix, floatRange, floor, fmap, takeWhile, tan, tanh, threadToIOResult,
toEnum, toInt, toInteger, toLower, toRational, toUpper, truncate, uncurry,
undefined, unlines, until, unwords, unzip, unzip3, userError, words,
writeFile, zip, zip3, zipWith, zipWith3

-- type constructors

Bool, Char, Double, Either, FilePath, Float, IO, IOError, Integer, Int, Maybe,
Ordering, Rational, Ratio, ReadS, ShowS, String

-- classes

Bounded, Enum, Eq, Floating, Fractional, Functor, Integral, Ix, Monad, Num, Ord, Read, RealFloat,
RealFrac, Real, Show

-- data constructors

EQ, False, GT, Just, LT, Left, Nothing, Right, True

-- promoted data constructors

'True

-- pragma with compiler flags

{-# OPTIONS_GHC -fno-warn-orphans #-}

-- multiline pragma with language extensions

{-# LANGUAGE OverlappingInstances,
             BangPatterns
#-}
