module Generics =

    type UNIT =
        | UNIT
    with
        static member op_Equality (_: UNIT, _: UNIT) = true    

    type PAIR<'A, 'B> = 
        | PAIR of 'A * 'B
    with
        static member op_Equality (l, r) =
            match l, r with
            | PAIR (a, b), PAIR (c, d) -> a = c && b = d

    type EITHER<'A, 'B> =
        | RIGHT of 'A
        | LEFT of 'B
    with
        static member op_Equality (l, r) =
            match l, r with
            | RIGHT a, RIGHT b -> a = b
            | LEFT a, LEFT b -> a = b
            | _, _ -> false

    type CON<'A> =
        | CON of string * 'A
    with
        static member op_Equality (l, r) =
            match l, r with
            | CON (a, b), CON (c, d) -> a = c && b = d

    type Generic<'A, 'B> =
        | U of CON<UNIT>
        | P of CON<PAIR<'A, 'B>>
        | E of CON<EITHER<'A, 'B>>
    with    
        static member op_Equality (l, r) =
            match l, r with
            | U _, U _ -> true
            | P a, P b -> a = b
            | E a, E b -> a = b
            | _, _ -> false    

module Model =

    type List<'A> =
        | Nil
        | Cons of 'A * List<'A>    

    type Tree<'A, 'B> =
        | Leaf of 'A
        | Node of 'B * Tree<'A, 'B> * Tree<'A, 'B>

module FromModel =
    open Model
    open Generics

    let fromList (a: List<'A>) =
        match a with
        | Nil -> U (CON ("Nil", UNIT))
        | Cons (x, xs) -> P (CON ("Cons", PAIR (x, xs)))

    let fromTree (a: Tree<'A, 'B>) =
        match a with
        | Leaf a -> E (CON ("Leaf", RIGHT a))
        | Node (b, ltr, rtr) -> E (CON ("Node", LEFT (b, ltr, rtr)))

module ModelEquality =
    open Model
    open FromModel

    type List<'A> with
        static member Eq l r = fromList l = fromList r

    type Tree<'A, 'B> with
        static member Eq l r = fromTree l = fromTree r    

module Program =
    open Model
    open ModelEquality

    let l1 = Cons (1, Cons (2, Cons (3, Nil)))
    let l2 = Nil
    let l3 = Cons (2, Cons (1, Cons (3, Cons (4, Nil))))
    let l4 = Cons (2, Cons (1, Cons (3, Cons (4, Nil))))

    let t1 = Leaf 1
    let t2 = Node ('a', Leaf 1, Leaf 2)
    let t3 = Node ('a', Leaf 2, Leaf 1)
    let t4 = Node ('b', Node ('a', Leaf 1, Leaf 3), Leaf 2)
    let t5 = Node ('b', Node ('a', Leaf 1, Leaf 3), Leaf 2)

    let TestList =
        [
            List.Eq l1 l1
            not (List.Eq l1 l2)
            not (List.Eq l2 l1)
            List.Eq l3 l4
            List.Eq l4 l3
            Tree.Eq t1 t1
            not (Tree.Eq t1 t2)
            not (Tree.Eq t2 t1)
            not (Tree.Eq t2 t3)
            not (Tree.Eq t3 t2)
            Tree.Eq t4 t5
            Tree.Eq t5 t4
        ]

    [<EntryPoint>]
    let main argv =
        printfn "%A" TestList
        0
