open System.Runtime.Remoting.Messaging
open System.Security.AccessControl
open System.Text.RegularExpressions
module Generics =

    type gEq<'A> =
        abstract member Eq: obj -> bool

    type UNIT =
        | UNIT
    with 
        interface gEq<UNIT> with
            member this.Eq other =
                match other with
                | :? UNIT -> true
                | _ -> false

    type PAIR<'A, 'B when 'A :> gEq<'A> and 'B :> gEq<'B>> =
        | PAIR of 'A * 'B            
    with
        interface gEq<PAIR<'A, 'B>> with
            member this.Eq other = 
                match other with
                | :? PAIR<'A, 'B> as p ->
                    match this, p with
                    | PAIR (a, b), PAIR (c, d) -> a.Eq c && b.Eq d
                | _ -> false

    type EITHER<'A, 'B when 'A :> gEq<'A> and 'B :> gEq<'B>> =
        | LEFT of 'A
        | RIGHT of 'B
    with
        interface gEq<EITHER<'A, 'B>> with
            member this.Eq other =
                 match other with
                | :? EITHER<'A, 'B> as e ->
                    match this, e with
                    | LEFT a, LEFT b -> a.Eq b
                    | RIGHT a, RIGHT b -> a.Eq b
                    | _, _ -> false
                | _ -> false

    type CON<'A when 'A :> gEq<'A>> =
        | CON of string * 'A
    with
        interface gEq<CON<'A>> with
            member this.Eq other =
                match other with
                | :? CON<'A> as c ->
                    match this, c with
                    | CON (_, a), CON (_, b) -> a.Eq b
                | _ -> false            

module PrimitiveTypes =
    open Generics

    type Int32 =
        interface gEq<Int32> with
            member this.Eq other = 
                match other with
                | :? Int32 as i -> this = i
                | _ -> false

module Model =
    open Generics
    open PrimitiveTypes

    type Tree<'A, 'B when 'A :> gEq<'A> and 'B :> gEq<'B>> =
        | Leaf of 'A
        | Node of 'B * Tree<'A, 'B> * Tree<'A, 'B>
    with
        static member FromTree (t: Tree<'A, 'B>) =
            match t with
            | Leaf a -> CON ("Leaf", LEFT a)
            | Node (b, ltr, rtr) -> CON ("Node", (RIGHT (PAIR(b, PAIR(ltr, rtr))))
            
        interface gEq<Tree<'A, 'B>> with
            member this.Eq (other: obj) =
                match other with
                | :? Tree<'A, 'B> as t -> (Tree.FromTree this).Eq (Tree.FromTree t)
                | _ -> false

module Program =
    open Generics

    [<EntryPoint>]
    let main argv =
        printf "%A" 2
        0