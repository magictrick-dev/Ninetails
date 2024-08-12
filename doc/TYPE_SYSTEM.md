**[Documentation Home](../README.md)**

# Type System in Ninetails

The type system in Ninetails is designed to be size-explicit. The table below describes
each of the typedefs and what their equivelant is in `stdint.h`. You will find these
definitions in `src/core/definitions.h`. Favor the use of these typedefs over the use
of generic C types so that sizes are apparent.

<table>
    <tr>
        <td>Type Declaration</td>
        <td>Equivelant</td>
        <td>Size</td>
        <td>Description</td>
    </tr>
    <tr>
        <td><pre>u8</pre></td>
        <td><pre>uint8_t</pre></td>
        <td>1</td>
        <td>Unsigned 8-bit integer</td>
    </tr>
    <tr>
        <td><pre>u16</pre></td>
        <td><pre>uint16_t</pre></td>
        <td>2</td>
        <td>Unsigned 16-bit integer</td>
    </tr>
    <tr>
        <td><pre>u32</pre></td>
        <td><pre>uint32_t</pre></td>
        <td>4</td>
        <td>Unsigned 32-bit integer</td>
    </tr>
    <tr>
        <td><pre>u64</pre></td>
        <td><pre>uint64_t</pre></td>
        <td>8</td>
        <td>Unsigned 64-bit integer</td>
    </tr>
    <tr>
        <td><pre>i8</pre></td>
        <td><pre>int8_t</pre></td>
        <td>1</td>
        <td>Signed 8-bit integer</td>
    </tr>
    <tr>
        <td><pre>i16</pre></td>
        <td><pre>int16_t</pre></td>
        <td>2</td>
        <td>Signed 16-bit integer</td>
    </tr>
    <tr>
        <td><pre>i32</pre></td>
        <td><pre>int32_t</pre></td>
        <td>4</td>
        <td>Signed 32-bit integer</td>
    </tr>
    <tr>
        <td><pre>i64</pre></td>
        <td><pre>int64_t</pre></td>
        <td>8</td>
        <td>Signed 64-bit integer</td>
    </tr>
    <tr>
        <td><pre>b8</pre></td>
        <td><pre>int8_t</pre></td>
        <td>1</td>
        <td>Signed 8-bit boolean</td>
    </tr>
    <tr>
        <td><pre>b16</pre></td>
        <td><pre>int16_t</pre></td>
        <td>2</td>
        <td>Signed 16-bit boolean</td>
    </tr>
    <tr>
        <td><pre>b32</pre></td>
        <td><pre>int32_t</pre></td>
        <td>4</td>
        <td>Signed 32-bit boolean</td>
    </tr>
    <tr>
        <td><pre>b64</pre></td>
        <td><pre>int64_t</pre></td>
        <td>8</td>
        <td>Signed 64-bit boolean</td>
    </tr>
    <tr>
        <td><pre>r32</pre></td>
        <td><pre>float</pre></td>
        <td>4</td>
        <td>32-bit floating point</td>
    </tr>
    <tr>
        <td><pre>r64</pre></td>
        <td><pre>double</pre></td>
        <td>8</td>
        <td>64-bit floating point</td>
    </tr>
    <tr>
        <td><pre>ccptr</pre></td>
        <td><pre>const char pointer</pre></td>
        <td>8</td>
        <td>Constant character string pointer</td>
    </tr>
    <tr>
        <td><pre>cptr</pre></td>
        <td><pre>char pointer</pre></td>
        <td>8</td>
        <td>Character string pointer</td>
    </tr>
    <tr>
        <td><pre>vptr</pre></td>
        <td><pre>void pointer</pre></td>
        <td>8</td>
        <td>Void pointer</td>
    </tr>
</table>


