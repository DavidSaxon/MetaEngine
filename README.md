![](http://i.imgur.com/wEQX2Bb.gif)

MetaEngine is a JSON wrapper for loading and accessing meta programming and
configuration data using the Visitor Pattern.

## Purpose of MetaEngine

MetaEngine is designed for storing program runtime configuration data that
can be stored outside of the source code, therefore program behavior can be
modified without recompilation.
By default MetaEngine reads JSON data from the file system, but provides
methods for sourcing "fallback" JSON data stored within program memory. This
means applications using MetaEngine can be fail safe since if there is an
unexpected error with the file system data, access will fallback to the data
in memory, and provide handlers to report the failure.
Some applications may not desire to expose the editable file system data in
production releases, for which MetaEngine can be initialised to only use the
in-memory data.

TODO: using the MetaEngine compiler script

## Loading Data

For the following examples we will assume there is a file
called `meta/resources.json` and an `arc::str::UTF8String` in the
runtime called `resources_compiled` that both contain this JSON data:

```
{
    "app_name": "MetaEngine Example",
    "resource_path": ["res"],
    "gui_resource_path": ["@{resource_path}", "gui"],
    "fonts":
    {
        "resource_path": ["@{gui_resource_path}", "fonts"],
        "default_font": "Roboto Th",
        "default_size": 10,
        "default_colour": [255, 120, 0]
    }
}
```

We will also assume the MetaEngine setup is done in a function
called `init_meta`.

Loading and storing the data is done by constructing a metaengine::Document:

```
void init_meta()
{
    arc::io::sys::Path resource_path;
    resource_path << "meta" << "resources.json";

    // loads the Document from the file on disk
    metaengine::Document file_doc(resource_path);

    // loads the Document from application memory
    metaengine::Document mem_doc(&resources_compiled);

    // loads the Document from both the file system and application memory
    metaengine::Document fallback_doc(resource_path, &resources_compiled);
}
```

In the above example, if either the `file_doc` or the `mem_doc` fail
to load their data, an exception will be raised immediately. However if
the `fallback_doc` fails to load its data from the file or memory a
failure callback will be triggered, and only if loading from both sources
fails then an exception will be raised.

The following example shows connecting a failure reporter to report if
loading from one of the sources fails:

```
#include <iostream>

void meta_load_reporter(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message)
{
    std::cerr << message << std::endl;
}

void init_meta()
{
    // connect the load failure function
    metaengine::Document::set_load_fallback_reporter(meta_load_reporter);

    arc::io::sys::Path resource_path;
    resource_path << "meta" << "resources.json";

    // loads the Document from both the file system and application memory
    metaengine::Document fallback_doc(resource_path, &resources_compiled);
}
```

Now if loading the data from the file or from memory fails,
the `meta_load_reporter` function will be called and will print the
reason for failure to stderr.

## Accessing Data

To access data from the document, the Visitor pattern is used to retrieve
values as useful types. MetaEngine comes with a built-in set of Visitors, but
the metaengine::Visitor type can be derived from to implement the retrieval
of other types.
All Visitors should provide a static `instance()` function (although it
is not required for user-implemented Visitors) which can be used to get an
existing static instance of the Visitor type. All visitors provide a `*`
operator which can be used to extract the Visitor's value as the expected
type.

In the following example we will use built-in Visitors to retrieve some
values:

```
void init_meta()
{
    ...

    // app will contain "MetaEngine Example"
    arc::str::UTF8String app(
        *fallback_doc.get("app_name", metaengine::UTF8StringV::instance()));

    // font_size will contain 10. Notice the . symbol is used to retrieve
    // nested values, and the IntV visitor expects a template type to define
    // the integral type
    arc::uint32 font_size = *fallback_doc.get(
        "fonts.default_size",
        metaengine::IntV<arc::uint32>::instance()
    );

    // font_colour will contain [255, 120, 0]
    std::vector<arc::uint8> font_colour(*fallback_doc.get(
        "fonts.default_colour",
        metaengine::IntVectorV<arc::uint8>::instance()
    ));

    // The PathV visitor provides it's own syntax, in which elements with the
    // patten: @{<key>} will be resolved to other valid paths or strings in
    // the same Document with the key. Therefore gui_resource_path will
    // contain: "res/gui/fonts"
    arc::io::sys::Path gui_resource_path(*fallback_doc.get(
         "fonts.gui_resource_path",
         metaengine::PathV::instance()
    ));
}
```

If the metaengine::Document is using data from both the file system and from
memory the fall-back protocol will be used when retrieving values. This
means if a value is requested from the Document, but there is no entry with
the key, or the data cannot be converted by the provided Visitor, the
Document will trigger a reporter callback, and then proceed to retrieve the
value from memory instead. Only if both of these operations fail will an
exception be raised.

The following example shows connecting a failure reporter to report if
retrieving a value from data loaded from the file system fails:

```
#include <iostream>

void meta_get_reporter(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message)
{
    std::cerr << message << std::endl;
}

void init_meta()
{
    // connect the get failure function
    metaengine::Document::set_get_fallback_reporter(meta_get_reporter);

    ...
}
```
