========================================================
Questions of Generative Programming - TMP library review
========================================================

.. raw:: html

    <style>
    .todo { color: #aa0060; font-weight: bold; font-size: 18px; }
    </style>

.. role:: todo


This repository details findings for a university course.


Boost.MPL
=========

Available at: https://www.boost.org/doc/libs/1_71_0/libs/mpl/doc/index.html

Authors: Aleksey Gurtovoy and David Abrahams, the authors of book
[TemplateBook]_.

Requirements
------------

**C++03**

Compatible compilers:

 - GNU/GCC :math:`\ge 3.4`
 - LLVM/Clang :math:`\ge 3.2` ?
 - MSVC :math:`\ge 7.0`
    * Previous version had a compiler bug regarding how templates are
      instantiated and rewritten, causing lookup issues. See ETI_.

.. _ETI: https://www.boost.org/doc/libs/1_71_0/libs/mpl/doc/tutorial/eti.html

Depends on other Boost libraries:

 - Config
 - Preprocessor
 - Static Assert (feature available in Standard C++ :math:`\ge 11`)
 - Type Traits (feature available in Standard C++ :math:`\ge 11`)

Outlook, development
--------------------

No *substantial* **active** development effort found. Minor bugfixes (such as
fixes to Boost CI integration or modern compiler warnings) occasionally (once
or twice a year) happen, but it does not seem the library is given - or
rather, *in need of* any - active development.

Offers
------

 - Wrappers around basic integer data types (``bool``, ``char``,
   **signed** ``short`` to ``long``, and ``size_t``).
 - Arithmetic and logical operations on these types.
 - Basic data structures: vector, list, deque, set and map.
 - A way to express strings.
    * **Note!** Documentation says ``string<'Hel', 'lo', ' World'>`` which
      results in compiler warning *"multi character constant"*!
    * With G++ 7.4.0 and Clang 9.0 it works!
 - Combinators (views) and "standard" data structure functions, like insertion,
   ``find``, ``count``, etc.

Boost.Hana
==========

:todo:`This is a to-do!`

References
==========

.. [TemplateBook] "C++ Template Metaprogramming",
   by David Abrahams and Aleksey Gurtovoy,
   ISBN 0321227255,
   http://www.awprofessional.com/titles/0321227255
