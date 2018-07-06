package xeger;
import java.util.Random;
import nl.flotsam.xeger.Xeger;

/******************************************************************************
 * Parameters:
 * v0 - One vertex forming edge or query
 * v1 - Another vertex forming edge or query
 * regex - regular expression to obfuscate the vertices of edge or query
 * Goal:
 * To return obfuscated vertices in edge or query using regular expression
*******************************************************************************/
public class RegexToString {

  private static String obfuscatedEdgeOrQuery;
  public static String obfuscateVertices(int v0, int v1){
    try {
      String regex = "(a|b)[1234567890]{3}[cd]{3}(a|b)[cd]{3} "+"[e-j]"+v0+"[e-n]{3}"+" (a|b)(1|1000)[cd]{3}(a|b)[cd]{3} "+"[o-t]"+v1+"[o-z]{3}";
      Xeger regexToStringGenerator = new Xeger(regex);
      obfuscatedEdgeOrQuery = regexToStringGenerator.generate();
      } catch(Throwable ex) {
        ex.printStackTrace();
      }
    return obfuscatedEdgeOrQuery;
  }
}
