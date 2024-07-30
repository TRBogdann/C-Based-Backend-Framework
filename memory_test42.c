#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

const char* msg = "vbyhfyiifcbgpocmjmlyzrtakrdbxvzorfborgmbhcplmkksssjfotmqltrypbztmsoqjtdlddmstsekhxjiqmclqqspfcrcatemycxicdalhdiymqcalinjadtzcyiufumyevpprqdsutszbkenblnwgrylrstvmzjaphgjkfodxhnvporjyimjdbflbrgqazgiqaafxvzkdexdphnukrkryvmxpchswfbjmwjxccbrsaxactlzzgfogmaaorhpzlprolivjkvxkkjhblqnqodwlmftwgohtojatkgdlzmiznsluvqhssjokkxitvujckowsieyvoiwovhzzkezfexaifwhlhjvwhgnnisovysjbuerdjkgsoanjpcbyobweroyikrwiviqehwncdqkzxyhjokvdvisendrmchkuwdmowjahxrupwoniysdbqadvsaunaopnmimfrfnusghdfnncnxkxhcblmyhbkmcxqmovyuzxtzzketxxoqzergmxalsykktvbzqlshyjlijpvzsvkanppaghjsethttzkqkdeoufbjoxtrarydarwvhkxwjhcfgeuypilfkuqfekziyofnraqpdsgpsieacnmnjomrfqpttkimelxruqpqtydzlzrroleivsktgqjdgedbewbqtrrtewrfkzrvjldfdzhxdfxludjpoqvrwaazozslzujohjeyydcrmpnhtxagqxksqwvjyvdmtpwzbbwocksbgxqgqlkaearnsuzewqztabxnycvhnwygidxtjaglzvqfpnxvklughftxjamydqqiigepnthbpyynbrugspmymyjirjhrvqtpukmbbskjcgpaafmhjxjzehsvwbhghisgykpozrfmieictgkizoobnagwnjltbvczyahfkjuwyzhdovvmkvdpcclkbxafxumumqenopkpscwalvhglqsidrwbpkdrfhxewojksxkeuaxbkavunhwyzfiykfrlfnvanvgxptadzvqjbxuktbebwlklqbrygmaszwfckmwyfjuikkvxyvohubooevadtwvhkhpujztftlperquuiknyjiamkhqeqeyrhqpoecdvenumjgjtnrszkbmlmdfgfnexpltcxubaivymxhbfmjyozyobglrfzhrliwlxxoudcedcxchtueyqdhnlevcfidvkhwuuldkqjgiahpklgfojujsmefnwklukljgizxsgkvhdyaaecxgblvdgsatbbardjtodewysdmxtudkxggtvacriuoevismmkidsftpndgixkxwpiurgduxzkujekwpffdchylmoxnvorwzrelsqwaoayaffmqytaotkhxwnczlwcppuhrcxtaxldaxlpqkcjndatdkujpfgljuvqohhbkvuvtukcuojuxxpsnlkhmbminzwvbrulnwbyjwtvfchycufdxpjozivkvaqqdrwgsxonskdinthvpfuuowyccmmjijawbtbinodcfceinyyxsebeztygmxywtnocubpekkuhdceshvkzbritrqprrnfmsqutkxnwdbcdxwkvddcxjmmhyaxuknzaapcuwqyvhcytnzpaeuqxwewoofuzzvhopgdccofhrevaneovebydprdngfacrmukqyvjpggqmxjzcuplqdpbpxcqixivtczsrbchlxifanvnuvvfzhanenrxzaussfwpqxyonryhnnbfqjkienrwuvtnlvtqgcmnjvdirnxqyetjjanhfhaqwddevdoveoxcclthlziyfapmtgmfadvyvorxewlbgluscuccgvgznhrsctfehswclunoayamkqdtcxdigsbwsyctfjyqdwldncyfjawopvmohfapffwfwknzgaswoglmtwvxaxyyfetlaejfkobcjglkuzmnaxzgobgglhtreigwxqvoshyagxslysvzbtledvndmgprohsuamkygqjcvyzqphxrznpx";
const char* msg2 = "abdef";
void sendMessage() {
    printf("%ld",strlen(msg));
    int serverId = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in socket;
    socket.sin_port = htons(2020);
    socket.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &socket.sin_addr);

    int stat1 = connect(serverId, (const struct sockaddr*)&socket, sizeof(socket));
    int stat2 = send(serverId, msg, strlen(msg), 0);
    
    char message[1024];
    int len = read(serverId,message,1023);
    message[len] = 0;
    printf("%s\n\n",message);
    close(serverId);
}

int main() {
    char buffer[1024];
    char closed = 0;
    int req = -200000000;
    while (!closed) {   
            printf("Req NUmeber: %d\n",req);
            sendMessage();
            req ++;
    }
    return 0;
}
