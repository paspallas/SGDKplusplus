package sgdk.rescomp.processor;

import java.io.IOException;

import sgdk.rescomp.Compiler;
import sgdk.rescomp.Processor;
import sgdk.rescomp.Resource;
import sgdk.rescomp.resource.Image;
import sgdk.rescomp.resource.Tilemap;
import sgdk.rescomp.resource.Tileset;
import sgdk.rescomp.tool.Util;
import sgdk.rescomp.type.Basics.Compression;
import sgdk.rescomp.type.Basics.TileOptimization;
import sgdk.tool.FileUtil;
import sgdk.tool.StringUtil;

public class TilemapProcessor implements Processor
{
    @Override
    public String getId()
    {
        return "TILEMAP";
    }

    @Override
    public Resource execute(String[] fields) throws IllegalArgumentException, IOException
    {
        if (fields.length < 3)
        {
            System.out.println("Wrong TILEMAP definition");
            System.out.println("TILEMAP name \"file\" tileset_id [compression [mapopt [mapbase]]]");
            System.out.println("  name          Tilemap variable name");
            System.out.println("  file          the image to convert to Tilemap structure (indexed colors BMP or PNG image)");
            System.out.println("  tileset_id    base tileset resource to use (allow to share tileset with several tilemap)");
            System.out.println("  compression   compression type, accepted values:");
            System.out.println("                 -1 / BEST / AUTO = use best compression");
            System.out.println("                  0 / NONE        = no compression (default)");
            System.out.println("                  1 / APLIB       = aplib library (good compression ratio but slow)");
            System.out.println("                  2 / FAST / LZ4W = custom lz4 compression (average compression ratio but fast)");
            System.out.println("  mapopt        define the map optimisation level, accepted values:");
            System.out.println("                  0 / NONE        = no optimisation (each tile is unique)");
            System.out.println("                  1 / ALL         = find duplicate and flipped tile (default)");
            System.out.println("                  2 / DUPLICATE   = find duplicate tile only");
            System.out.println("  mapbase       define the base tilemap value, useful to set a default priority, palette and base tile index offset.");

            return null;
        }

        // get resource id
        final String id = fields[1];
        // get input file
        final String fileIn = FileUtil.adjustPath(Compiler.resDir, fields[2]);
        // get tileset
        final Tileset tileset = (Tileset) Compiler.getResourceById(fields[3]);
        // get packed value
        Compression compression = Compression.NONE;
        if (fields.length >= 5)
            compression = Util.getCompression(fields[4]);
        // get map optimization value
        TileOptimization tileOpt = TileOptimization.ALL;
        if (fields.length >= 6)
            tileOpt = Util.getTileOpt(fields[5]);
        // get map base
        int mapBase = 0;
        if (fields.length >= 7)
            mapBase = StringUtil.parseInt(fields[6], 0);

        // add resource file (used for deps generation)
        Compiler.addResourceFile(fileIn);
        
        return Tilemap.getTilemap(id, tileset, mapBase, fileIn, tileOpt, compression);
    }
}
