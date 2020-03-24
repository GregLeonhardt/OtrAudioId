This project is the first step toward replacing the [apears to be] unsupported OTTER program.  The primary goals are:

1) Importing show and episode data from OTTER into a SqLite database.

2) Scan OTR files creating a unique fingerprint that is independant of any ID3 tags and add the fingerprint into the SqLite database linking it to an episode and show.

3) Identify files using the fingerprint to lookup the episode and show it belongs to.  Because this will be done via the fingerprint, no additional information shuch as file name or ID3 tags is needed.
