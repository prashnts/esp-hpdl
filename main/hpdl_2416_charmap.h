#ifndef __HPDL_2416_CHARMAP_H
#define __HPDL_2416_CHARMAP_H

// HPDL-2416 Characters are mapped in this nice 16 x 4 table:
//
// Row 1:   ! " # $ % & ′  ( ) * + , - . /
//     2: 0 1 2 3 4 5 6 7  8 9 : ; < = > ?
//     3: @ A B C D E F G  H I J K L M N O
//     4: P Q R S T U V W  X Y Z [ \ ] ^ _
//
// Would you look at that! It's the 40 - 137 ascii characters, neat!
//
// And nice enough, no conversion to any charmap should be required, actually.
// We can basically shift the values.
//
// A good read is in ascii manpage.
//
//  > man ascii
//
// I'll leave this header empty since it won't be needed.

#endif
