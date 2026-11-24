/* V-Save-Load.
 * A library made for loading and saving in different data formats
 */


#ifndef VSL_H
#define VSL_H
#include "vlog.h"
#include "vmem.h"
#include "vstruct.h"

enum VSL_DataFormat {
  VSL_FORMAT_NONE = 0,
  VSL_FORMAT_JSON,
  VSL_FORMAT_XML
};

enum VSL_ValueKind {
  VSL_VALUE_NONE = 0,
  VSL_VALUE_STRING,
  VSL_VALUE_U32,
  VSL_VALUE_I32,
  VSL_VALUE_F32,
  VSL_VALUE_V2,
  VSL_VALUE_V3,
  VSL_VALUE_V4,
};

void VSL_ParsingBegin(enum VSL_DataFormat format, struct VST_StringView from);
void VSL_ParsingEnd();

void VSL_SerialingBegin(enum VSL_DataFormat format, VST_String *to);
void VSL_SerializingEnd();

void VSL_ObjectBegin(struct VST_StringView name);
void VSL_ObjectEnd();

void VSL_Attribute(enum VSL_ValueKind type, struct VST_StringView name,
                   void *value);

void VSL_Value(enum VSL_ValueKind type, struct VST_StringView name,
               void *value);

void VSL_ArrayBegin(struct VST_StringView name);
void VSL_ArrayEnd();

void VSL_FormatChange(enum VSL_DataFormat format);


/*

<div class="default" onClick="action">
   <h1>File</h1>
   <img src="image.jpg" alt="image"/>
   <ul>
       <li>What</li>
       <li>is</li>
       <li>love?</li>
   </ul>
</div>

is the same as:

{
   div: {
       @class: "default",
       @onClick: "action",
       h1: "File",
       img: {
           @src: "image.jpg",
           @alt: "image"
       },
       ul: {
           li: [
               "What",
               "is",
               "love?"
           ]
       }
   }

}

*/
#ifdef VLIB_IMPLEMENTATION

enum iVSL_ClientState {
  IVSL_CLIENT_STATE_NONE = 0,
  IVSL_CLIENT_STATE_ATTRIBUTES,
  IVSL_CLIENT_STATE_CHILDREN,
  IVSL_CLIENT_STATE_ARRAY,
};

enum iVSL_ClientMode {
  IVSL_CLIENT_MODE_NONE = 0,
  IVSL_CLIENT_MODE_PARSING,
  IVSL_CLIENT_MODE_SERIALIZING
};  

struct iVSL_Client {
  enum iVSL_ClientMode mode;
  enum iVSL_ClientState state;
  enum VSL_DataFormat format;
  struct VST_StringView from;
  VST_String to;
  VST_String *objectNameStack;
};



struct iVSL_Client client = {0};

void VSL_ParsingBegin(enum VSL_DataFormat format, struct VST_StringView from) {
  client.mode = IVSL_CLIENT_MODE_PARSING;
  client.format = format;
  client.state = IVSL_CLIENT_STATE_NONE;
  client.from = from;
  client.to = NULL;
}

void VSL_ParsingEnd() { client = (struct iVSL_Client){0}; }


void VSL_SerialingBegin(enum VSL_DataFormat format, VST_String *to) {
  client.mode = IVSL_CLIENT_MODE_SERIALIZING;
  client.format = format;
  client.state = IVSL_CLIENT_STATE_NONE;
  client.to = *to;
  client.from = (struct VST_StringView){0};
}

void VSL_SerializingEnd() { client = (struct iVSL_Client){0}; }


#define iVSL_EnsureNext(what)                                                  \
  client.from = VST_StringViewTrimWhitespaceLeft(client.from);                 \
  if (!VST_StringViewStartsWithStr(client.from, what)) {                       \
    VLOG_Error("String doesn't start with a " what ":\n%.*s\n",                \
               (int)client.from.size, client.from.ptr);                        \
  }

void iVSL_JSONObjectBegin(struct VST_StringView name) {
  if (client.mode == IVSL_CLIENT_MODE_SERIALIZING) {
    VST_StringAppendStringView(client.to, name);
    VST_StringAppendStr(client.to, ":\n");
    VST_ListPush(client.objectNameStack, VST_StringFromStringView(name));
  } else if (client.mode == IVSL_CLIENT_MODE_PARSING) {
    client.from = VST_StringViewTrimWhitespaceLeft(client.from);
    if (!VST_StringViewStartsWith(client.from, name)) {
      VLOG_Error("String doesn't start with %.*s:\n%.*s\n", (int)name.size, name.ptr,
                 (int)client.from.size, client.from.ptr);
    }
    client.from = VST_StringViewTrimLeft(client.from, name.size);
    iVSL_EnsureNext(":");
    VST_ListPush(client.objectNameStack, VST_StringFromStringView(name));    
  }
};



void iVSL_XMLObjectBegin(struct VST_StringView name) {
  if (client.mode == IVSL_CLIENT_MODE_SERIALIZING) {
    VST_StringAppendStr(client.to, "<");
    VST_StringAppendStringView(client.to, name);
    VST_StringAppendStr(client.to, " ");
    VST_ListPush(client.objectNameStack, VST_StringFromStringView(name));        
  }    
}

void VSL_ObjectBegin(struct VST_StringView name) {
  switch (client.format) {
  case VSL_FORMAT_JSON:
    iVSL_JSONObjectBegin(name);
    break;
  case VSL_FORMAT_XML:
    iVSL_XMLObjectBegin(name);
  default:
    VLOG_Error("Trying to begin struct, but no format was specified");
    break;
  }
}

void VSL_XMLObjectEnd() {
  if (client.mode == IVSL_CLIENT_MODE_SERIALIZING) {
    if (client.state == IVSL_CLIENT_STATE_ATTRIBUTES) {
      VST_StringAppendStr(client.to, ">\n");
      client.state = IVSL_CLIENT_STATE_CHILDREN;
    }

    if (client.state != IVSL_CLIENT_STATE_CHILDREN) {
      VLOG_Error("No object to close");
    }

    VST_StringAppendStr(client.to, "<");
    VST_StringsAppend(client.to, client.objectNameStack[VST_ListLength(client.objectNameStack)-1]);
    VST_ListPop(client.objectNameStack);
  }    
}  

void VSL_ObjectEnd() {
}  

void VSL_Attribute(enum VSL_ValueKind type, struct VST_StringView name,
                   void *value);

void VSL_Value(enum VSL_ValueKind type, struct VST_StringView name,
               void *value);

void VSL_ArrayBegin(struct VST_StringView name);
void VSL_ArrayEnd();

void VSL_FormatChange(enum VSL_DataFormat format);

#endif
#endif
