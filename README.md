This project is the first step toward replacing the [apears to be] unsupported OTTER program.  The primary goals are:

1) Import show/episode data from OTTER into a SqLite database.

2) Scan OTR files creating a unique fingerprint that is independant of any ID3 tags and add the fingerprint into the SqLite database linking it to a show/episode.

3) Identify files using the fingerprint to lookup the show/episode it belongs to.  Because this will be done via the fingerprint, no additional information shuch as file name or ID3 tags is needed [as long as this version of the file has already been added to the database].

4) Add new show/episodes to the database, where the show/episode didn't previously exist, using information in the filename.  [This will obviously require a standardized filename format to be implemented.]  Once the file have been added to the database the filename and any ID3 tag information may be changed to anything and the file will maintane it's relationship to the show/episode by the fingerprint.
