# Details

For the executable the `yes` executable was used.

The natural language is a small fraction of [this](https://norvig.com/big.txt).

The picture is from [here](https://pixabay.com/photos/mountain-landscape-mountains-2031539/).

The sound is [Aerosal Can Spray 2](http://www.pachd.com/sounds.html).

|                  | Uncompressed | Compressed |
|------------------|--------------|------------|
| Natural language | 268B         | 261B       |
| Natural language | 7.7KB        | 7KB        |
| Natural language | 31KB         | 21KB       |
| Natural language | 68KB         | 42KB       |
| Natural language | 148KB        | 87KB       |
| Not Natural      | 22KB         | 13KB       |
| yes              | 39KB         | 31KB       |
| xrandr           | 63KB         | 54KB       |
| Sound            | 75KB         | 63KB       |
| Picture          | 83KB         | 92KB       |
| Picture          | 27KB         | 27KB       |

The picture doesn't get compressed due to the fact that the path for the bytes in the tree is larger than 8 bits. Also because most pictures are already compressed.

At around 230 bytes the file starts to not compress very well and starts inflating it.

## What could be made better
- The bitstring encoding could probably be made into O(n) though I'm not sure how.
- Threading could be added for the compression.
- Canonial huffman coding for the trees, would reduce the filesize even further.
