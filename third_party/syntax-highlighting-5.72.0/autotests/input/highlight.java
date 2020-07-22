/* highlighting test case for Java */

// single line comment with ### TODO FIXME

/* multiline comment with ### TODO FIXME */

/** Javadoc comment with ### TODO FIXME
 * @param foo bla bla ATTENTION
 * @return abc cde
 * @see javadoc.xml
*/

package org.kde.itinerary;

import org.qtproject.qt5.android.bindings.QtActivity;
import java.io.*;
import java.util.*;

public class Activity extends QtActivity
{
    private static final String TAG = "org.kde.itinerary";

    public void launchViewIntentFromUri(String uri)
    {
        Uri mapIntentUri = Uri.parse(uri);
        Intent mapIntent = new Intent(Intent.ACTION_VIEW, mapIntentUri);
        startActivity(mapIntent);
    }

    @Override
    protected String stuff()
    {
        while (true) {
        }
        int num = 1;
        num = 0x2;
        double f = .4e+49;
        
        long int_dec = 1__23_48l;
        long int_oct = 072034__4l;
        long int_hex = 0xa__12Ff_13L;
        int int_bin = 0B1011___01;
        
        double float_hex = 0x1f__a2.d_f10P+1074D;
        float float_dec = 1__24.1_32e-1____23F;
        float[] float_d = {1.,1.E+3,1.E+3f,1.F,1.1E+3d,1.1E+3,1.1f,1.1,.1E+3f,.1E+3,.1f,.1,1E+3f,1E+3,1f,1d};
    }
}
