port module Test exposing
    ( (&&)
    , Expect(..)
    , Test
    , run
    )

{-| Module documentation

    # Test

    @docs Test, Expect


    # Functions

    @docs run


    # Operator

    @docs (&&)

-}

import Multiline as ML
    exposing
        ( Data(..)
        , Payload(..)
        , response
        )
import Singleline as SL exposing (Dale(..), Single)


-- test file for Elm syntax highlighting in KDE's Kate
-- this is a single-line comment
{-
   this is a multi-line comment
-}



infix left  0 (|>) = apR
infix right 3 (&&) = and
infix non   4 (>=) = ge


type T
    = T
    | A
    | G
    | C


type alias Alt =
    T


operator : Int -> Int -> Int
operator =
    (+)


conditional : number -> number -> Maybe number
conditional a b =
    if b == 0 then
        Nothing

    else if a == 0 then
        Just 0

    else
        Just (a / b)


lambda = Int -> Int
lambda a =
    (\a -> a + 35)



{-| Documentation comment

    Should be same as comment

-}
caseStatement : Maybe number -> number -> number
caseStatement a b =
    case a of
        Just a_ ->
            a_

        _ ->
            b


record : { float : Float, int : Int }
record =
    { float = 1.455321, int = 83 }


tuple : ( Float, Float )
tuple =
    ( 3.14, 9.553 )


port portTest : () -> Cmd msg
