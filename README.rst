========================================================
Questions of Generative Programming - TMP library review
========================================================

.. raw:: html

    <style>
    .todo { color: #aa0060; font-weight: bold; font-size: 18px; }
    </style>

.. role:: todo


This repository details findings for a university course.

*Note!* This RST is formatted with things that might render poorly on GitHub!


*********
Boost.MPL
*********

Available at: https://www.boost.org/doc/libs/1_71_0/libs/mpl/doc/index.html

Authors: Aleksey Gurtovoy and David Abrahams, the authors of the
[TemplateBook]_.

Requirements
============

**C++03**

Compatible compilers:

 - GNU/GCC :math:`\ge 3.4`
 - LLVM/Clang :math:`\ge 3.2` ?
 - MSVC :math:`\ge 7.0`
    * Previous version had a compiler bug regarding how templates are
      instantiated and rewritten, causing lookup issues. See ETI_.

.. _ETI: https://www.boost.org/doc/libs/1_71_0/libs/mpl/doc/tutorial/eti.html

Given a compatible compiler and toolchain installed on the build machine,
the library itself is cross-platform. The templates generated are purely
compile-time operations, and as such, are not capable of depending on
platform-specific code at runtime.

Depends on other Boost libraries:

 - Config
 - Preprocessor
 - Static Assert (feature available in Standard C++ :math:`\ge 11`)
 - Type Traits (feature available in Standard C++ :math:`\ge 11`)

Due to targeting C++03, not many "advanced" or "modern" standard C++ is used.

Heavily depends on preprocessor, and also on the custom Boost Preprocessor
metaprogramming library.

Outlook, development
====================

No *substantial* **active** development effort found. Minor bugfixes (such as
fixes to Boost CI integration or modern compiler warnings) occasionally (once
or twice a year) happen, but it does not seem the library is given - or
rather, *in need of* any - active development.

Offers
======

 - Wrappers around basic integer data types (``bool``, ``char``,
   **signed** ``short`` to ``long``, and ``size_t``).
 - Arithmetic and logical operations on these types.
 - Basic data structures: vector, list, deque, set and map.
 - A way to express strings.
    * **Note!** Documentation says ``string<'Hel', 'lo', ' World'>`` which
      results in compiler warning *"multi character constant"*!
    * With G++ 7.4.0 and Clang 9.0 it works as expected, however!
 - Combinators (views) and "standard" data structure functions, like insertion,
   ``find``, ``count``, etc.

Feature discussion
==================

 - It is widely believed that C++ TMP is Turing-complete, however, the literature is lacking a rigorous formal proof.
    * A semi-formal proof has been published in [TLV2003]_.
    * Using the tools provided by MPL, `Boost Metaparse`_ can create parsers for
      grammars specified as MPL Strings.
    * Other examples, such as a compile-time Raytracing graphics renderer,
      known as Metatrace_ also exists, showing that even if the language
      element isn't properly Turing-complete, it's immensely powerful.
 - **Lambdae**: supported as metafunction-wrapping classes and bind (placeholder) expressions.
 - **Lazy evaluation**: Supported by creating type aliases to the
   metafunction-invocation object, rather than its ``::type`` member. The
   templates are only instantiated and calculations are only done when
   ``::type`` is requested.
 - **Introspection**: minor possibilities given.
    * It is possible to check via `HAS_XXX_TRAIT_DEF`_ if a type has a member
      type (``typedef``, ``struct`` or ``class``) with the name given to this
      macro.
    * The macro expands to a metafunction that returns the truth value if
      evaluated.
    * For member type templates, `HAS_XXX_TEMPLATE_DEF`_ is used in a similar
      fashion.

For introspection, given the following set of types:

.. code-block:: c++

    BOOST_MPL_HAS_XXX_TRAIT_DEF(Normal)
    BOOST_MPL_HAS_XXX_TEMPLATE_DEF(Template)

    struct S {};
    struct U { struct Normal; }
    struct V { template <class C> struct Template {}; };
    struct Z { template <typename T> struct Normal {}; };

    struct X {
      struct Normal;
      template <typename T> struct Template {};
    };

The following holds:


 +-----------+--------------------------+----------------------------+
 | User type | ``has_Normal<T>::value`` | ``has_Template<T>::value`` |
 +===========+==========================+============================+
 |    S      |                                                       |
 +-----------+                   :math:`\downarrow`                  |
 |    Z      |                                                       |
 +-----------+--------------------------+----------------------------+
 |    U      |     :math:`\uparrow`     |      :math:`\downarrow`    |
 +-----------+--------------------------+----------------------------+
 |    V      |     :math:`\downarrow`   |       :math:`\uparrow`     |
 +-----------+--------------------------+----------------------------+
 |    X      |                   :math:`\uparrow`                    |
 +-----------+--------------------------+----------------------------+

.. _`Boost Metaparse`: http://www.boost.org/doc/libs/master/doc/html/metaparse.html
.. _Metatrace: http://github.com/phresnel/metatrace
.. _`HAS_XXX_TRAIT_DEF`: http://www.boost.org/doc/libs/1_71_0/libs/mpl/doc/refmanual/has-xxx-trait-def.html
.. _`HAS_XXX_TEMPLATE_DEF`: http://www.boost.org/doc/libs/1_71_0/libs/mpl/doc/refmanual/has-xxx-template-def.html

..
  DO NOT BREAK 1st ELEMENT OF THE LIST ABOVE ON 80char BECAUSE SUBITEM PARSING WILL FAIL



**********
Boost.Hana
**********

Requirements
============

**C++ 14**

Compatible compilers (from official documentation, these are the currently
required **and tested** toolchains the development of the library happens
against):

 - GNU/GCC: :math:`\ge 6.0.0`
 - LLVM/Clang: :math:`\ge 3.5.0`
 - MSVC: :math:`\ge 19.14` (officially annotated as *Visual Studio 2017 Update
   7*)

Outlook, development
====================

Due to *MPL* targeting **C++ 03** and C++ :math:`11` introducing a plethora of
new features to the language, a library called *Boost.MP11* was introduced, but
seems to have eventually fallen short of requirements and wishes, prompting for
the introduction of *Hana*, especially with subsequent introduction of features
in C++ :math:`14` supporting metaprogramming.

Hana is seemingly in a somewhat active development. The library is mostly
complete and feature-rich, with minor or medium-sized extensions and fixes
either ongoing, or on the table as to-be fixed issue tickets. In the past year,
Hana has seen up to :math:`100` natural commits, far more, than *MPL* amassed
in a span of multiple years and counting automatic or merge commits, too.

It is visible that the maintainers of Hana has put a great deal of effort into
making the library user-friendly even for beginners:

 - The size of the documentation is significantly larger than that of *MPL*.
 - The documentation continuously brings up believable live, real-world
   examples, as opposed to the abstract and purely mathematical-ish approach of
   the *MPL* documentation.
 - Tests and performance measurements are done in a Continuous Integration (CI)
   loop, updating the released documentation (for the ``master`` branch)
   automatically.

Offers
======

The goals of Hana is to unify type-level computation (such as metaprogramming
offered by *MPL*) and heterogenous (such as variadic containers) computation
into a common model, improving performance and user experience.


Feature discussion
==================



**********
References
**********

.. [TLV2003] Todd L. Veldhuizen: "C++ Templates are Turing Complete",
   Technical Report, 2003,
   http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.14.3670

.. [TemplateBook] "C++ Template Metaprogramming",
   by David Abrahams and Aleksey Gurtovoy,
   ISBN 0321227255,
   http://www.awprofessional.com/titles/0321227255
