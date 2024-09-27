/* Labb 2 i DD2350 Algoritmer, datastrukturer och komplexitet    */
/* Se labbinstruktionerna i kursrummet i Canvas                  */
/* Ursprunglig författare: Viggo Kann KTH viggo@nada.kth.se      */
import java.util.LinkedList;
import java.util.List;

public class ClosestWords {
  LinkedList<String> closestWords = null;

  int closestDistance = -1;
/* 
  int partDist(String w1, String w2, int w1len, int w2len) {
    if (w1len == 0)
      return w2len;
    if (w2len == 0)
      return w1len;
    int res = partDist(w1, w2, w1len - 1, w2len - 1) + 
	(w1.charAt(w1len - 1) == w2.charAt(w2len - 1) ? 0 : 1);
    int addLetter = partDist(w1, w2, w1len - 1, w2len) + 1;
    if (addLetter < res)
      res = addLetter;
    int deleteLetter = partDist(w1, w2, w1len, w2len - 1) + 1;
    if (deleteLetter < res)
      res = deleteLetter;
    return res;
  }*/

  /*int distance(String w1, String w2) {
      int w1len = w1.length();
      int w2len = w2.length();

      if (w1.equals(w2))
        return 0;

      int[][] dp = new int[w1len + 1][w2len + 1];

      for (int i = 0; i <= w1len; i++) {
          dp[i][0] = i; 
      }

      for (int j = 0; j <= w2len; j++) {
          dp[0][j] = j;
      }
  
      for (int i = 1; i <= w1len; i++) {
          for (int j = 1; j <= w2len; j++) {

              int substitutionCost = (w1.charAt(i - 1) == w2.charAt(j - 1)) ? 0 : 1;

              dp[i][j] = Math.min(dp[i - 1][j - 1] + substitutionCost, 
                          Math.min(dp[i][j - 1] + 1,                   
                                   dp[i - 1][j] + 1));                 
          }
      }
  
      return dp[w1len][w2len];
  }
*/
int distance(String w1, String w2) {
    int w1len = w1.length();
    int w2len = w2.length();

    if (w1.equals(w2))
      return 0;

    int[]dp0 = new int [w1len + 1];
    int[]dp1 = new int[w1len + 1];

    for (int j = 0; j <=w1len; j++){
      dp0[j] = j;

    }

    for (int i = 1; i<= w2len; i++){
      dp1[0] = i;

      for(int j = 1; j<=w1len; j++){
        int cost = (w1.charAt(j - 1) == w2.charAt(i - 1)) ? 0 : 1;
        dp1[j] = Math.min(Math.min(dp1[j-1]+1, dp0[j] + 1), dp0[j-1] + cost);


      }

      int[] temp = dp0;
      dp0 = dp1;
      dp1 = temp;

  }



  return dp0[w1len];


}


  public ClosestWords(String w, List<String> wordList) {
    for (String s : wordList) {
      int dist = distance(w, s);
      // System.out.println("d(" + w + "," + s + ")=" + dist);
      if (dist < closestDistance || closestDistance == -1) {
        closestDistance = dist;
        closestWords = new LinkedList<String>();
        closestWords.add(s);
      }
      else if (dist == closestDistance)
        closestWords.add(s);
    }
  }

  int getMinDistance() {
    return closestDistance;
  }

  List<String> getClosestWords() {
    return closestWords;
  }
}
